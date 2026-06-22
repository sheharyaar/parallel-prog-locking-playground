/*
 * Throughput benchmark for the Bakery lock: lock acquisitions per second as the
 * thread count rises. Bakery is expected to scale badly — each acquire reads
 * O(n) shared state in the number of threads — so sweeping threads (1, 2, 4,
 * 8, …) should show throughput stall or drop. That's the lesson.
 *
 * Plumbing is provided. The TODO (you) spot is the operation being timed.
 */
#define _POSIX_C_SOURCE 200809L
#include "bakery.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static bakery_lock_t *lock;
static int iters;

typedef struct { int tid; } worker_arg_t;

static void *worker(void *arg) {
    int tid = ((worker_arg_t *)arg)->tid;
    (void)tid;  /* remove once you use tid below */

    for (int i = 0; i < iters; i++) {
        /* TODO (you): the operation we're timing.
         *   - acquire the lock on behalf of `tid`
         *   - (empty critical section — we measure lock cost, not work done)
         *   - release the lock */
    }
    return NULL;
}

int main(int argc, char **argv) {
    int nthreads = (argc > 1) ? atoi(argv[1]) : 4;
    iters        = (argc > 2) ? atoi(argv[2]) : 200000;
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

    long total_ops = (long)nthreads * iters;
    printf("threads=%d  acquisitions=%ld  time=%.3f s  throughput=%.0f acq/s\n",
           nthreads, total_ops, secs, secs > 0 ? total_ops / secs : 0.0);

    bakery_destroy(lock);
    free(threads);
    free(args);
    return 0;
}
