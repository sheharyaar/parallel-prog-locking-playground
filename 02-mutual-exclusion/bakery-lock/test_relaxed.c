/*
 * Stage 2 — "the relaxation bites." Same mutual-exclusion workload as
 * test_bakery.c (the counter doesn't lie), but run as MANY short trials and
 * tallied, because a weak-ordering bug is rare per run: one long run can pass
 * by luck and tell you nothing. Repeat it a thousand times and the lost
 * increments show up as failed trials.
 *
 * Compare the two:
 *   - Stage 1 lock (seq_cst): expect 0 failed trials, however many you run.
 *   - Stage 2 lock (relaxed): expect some trials to come up short — that
 *     deficit is two threads in the critical section at once.
 *
 * The plumbing (arg parsing, the trial loop, spawn/join, tally, reporting) is
 * provided. The same two spots as Stage 1 are yours, marked TODO (you):
 *   1. the critical section in worker()
 *   2. the per-trial correctness invariant in run_trial()
 */
#define _POSIX_C_SOURCE 200809L
#include "bakery.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Non-atomic on purpose: the lock is the only thing protecting it. Reset to 0
 * at the start of every trial. */
static long counter = 0;

static bakery_lock_t *lock;
static int iters;

typedef struct { int tid; } worker_arg_t;

static void *worker(void *arg) {
    int tid = ((worker_arg_t *)arg)->tid;
    (void)tid;  /* remove once you use tid below */

    for (int i = 0; i < iters; i++) {
        /* TODO (you): the critical section — identical to Stage 1.
         *   - acquire the relaxed lock on behalf of `tid`
         *   - counter++;            (plain, non-atomic)
         *   - release the lock
         * The lock is the protection; keep the increment plain. */
    }
    return NULL;
}

/* Returns 1 if the trial's final counter is correct, 0 if increments were lost.
 * `*out_counter` receives the observed total so main() can report the worst
 * deficit. */
static int run_trial(int nthreads, long *out_counter) {
    counter = 0;

    lock = bakery_create(nthreads);
    if (!lock) {
        fprintf(stderr, "bakery_create returned NULL — is bakery_relaxed.c implemented?\n");
        exit(2);
    }

    pthread_t    *threads = malloc((size_t)nthreads * sizeof *threads);
    worker_arg_t *args    = malloc((size_t)nthreads * sizeof *args);
    if (!threads || !args) { perror("malloc"); exit(2); }

    for (int i = 0; i < nthreads; i++) {
        args[i].tid = i;
        if (pthread_create(&threads[i], NULL, worker, &args[i]) != 0) {
            perror("pthread_create");
            exit(2);
        }
    }
    for (int i = 0; i < nthreads; i++)
        pthread_join(threads[i], NULL);

    bakery_destroy(lock);
    free(threads);
    free(args);

    *out_counter = counter;

    /* TODO (you): the per-trial invariant. What must hold for this trial to
     * count as correct? Decide from `counter` and `nthreads * iters`. */
    int ok = 0;  /* <- you replace this */
    return ok;
}

int main(int argc, char **argv) {
    int nthreads = (argc > 1) ? atoi(argv[1]) : 4;
    iters        = (argc > 2) ? atoi(argv[2]) : 10000;
    int trials   = (argc > 3) ? atoi(argv[3]) : 1000;
    if (nthreads < 1 || iters < 1 || trials < 1) {
        fprintf(stderr, "usage: %s [nthreads>=1] [iters>=1] [trials>=1]\n", argv[0]);
        return 2;
    }

    long expected   = (long)nthreads * iters;
    int  failed     = 0;
    long worst       = expected;   /* smallest counter seen across trials */

    for (int t = 0; t < trials; t++) {
        long got;
        if (!run_trial(nthreads, &got)) {
            failed++;
            if (got < worst) worst = got;
        }
    }

    printf("threads=%d iters=%d trials=%d  expected=%ld/trial\n",
           nthreads, iters, trials, expected);
    printf("failed=%d/%d trials", failed, trials);
    if (failed) printf("  worst deficit=%ld (counter dropped to %ld)",
                       expected - worst, worst);
    printf("\n");

    /* The Stage 2 lesson is in `failed`: a correct (seq_cst) lock gives 0; a
     * relaxed one that loses mutual exclusion gives some. */
    printf("%s\n", failed ? "BROKEN (as designed for Stage 2)" : "no violations observed");
    return failed ? 1 : 0;
}
