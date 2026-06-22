/*
 * Mutual-exclusion test for the Bakery lock: "the counter doesn't lie."
 *
 * The plumbing (arg parsing, thread spawn/join, timing, reporting) is provided.
 * Two spots are the actual exercise, marked TODO (you):
 *   1. the critical section in worker()
 *   2. the correctness invariant in main()
 */
#define _POSIX_C_SOURCE 200809L
#include "bakery.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Shared and NON-ATOMIC on purpose: the lock must be the only thing keeping two
 * threads off it. If the lock is correct, every increment lands and the final
 * value is exactly threads*iters. If it's broken, increments are lost to the
 * read-modify-write race and the total comes up short.
 */
static long counter = 0;

static bakery_lock_t *lock;
static int iters;

typedef struct { int tid; } worker_arg_t;

static void *worker(void *arg) {
    int tid = ((worker_arg_t *)arg)->tid;
    (void)tid;  /* remove once you use tid below */

    for (int i = 0; i < iters; i++) {
        /* TODO (you): the critical section — the heart of this test.
         *   - acquire the lock on behalf of `tid`
         *   - counter++;            (plain, non-atomic)
         *   - release the lock
         * Keep the increment a plain `counter++`; the lock is the protection,
         * not an atomic. */
    }
    return NULL;
}

int main(int argc, char **argv) {
    int nthreads = (argc > 1) ? atoi(argv[1]) : 8;
    iters        = (argc > 2) ? atoi(argv[2]) : 100000;
    if (nthreads < 1 || iters < 1) {
        fprintf(stderr, "usage: %s [nthreads>=1] [iters>=1]\n", argv[0]);
        return 2;
    }

    lock = bakery_create(nthreads);
    if (!lock) {
        fprintf(stderr, "bakery_create returned NULL — is bakery.c implemented?\n");
        return 2;
    }

    pthread_t    *threads = malloc((size_t)nthreads * sizeof *threads);
    worker_arg_t *args    = malloc((size_t)nthreads * sizeof *args);
    if (!threads || !args) { perror("malloc"); return 2; }

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    for (int i = 0; i < nthreads; i++) {
        args[i].tid = i;
        if (pthread_create(&threads[i], NULL, worker, &args[i]) != 0) {
            perror("pthread_create");
            return 2;
        }
    }
    for (int i = 0; i < nthreads; i++)
        pthread_join(threads[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &t1);
    double secs = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9;

    long expected = (long)nthreads * iters;
    printf("threads=%d iters=%d  counter=%ld  expected=%ld  (%.3f s)\n",
           nthreads, iters, counter, expected, secs);

    /* TODO (you): the invariant. What must hold for the lock to be correct?
     * Decide PASS/FAIL from `counter` and `expected`, and set `ok`. */
    int ok = 0;  /* <- you replace this */

    bakery_destroy(lock);
    free(threads);
    free(args);

    printf("%s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}
