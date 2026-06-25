// Harness for the lower-bound-on-locations demo (book §2.9).
//
// Two phases, on purpose:
//   1. STRESS  -- threads run free. Your lock will usually LOOK correct,
//      because the covering interleaving almost never happens by chance.
//      This is the "but a thread checks before entering!" intuition in action.
//   2. FORCED  -- the controller drives the exact covering interleaving from
//      the book and catches two threads in the critical section.
//
// You own (marked TODO (you)): the stress invariant + its parameters, and the
// forced-schedule script (the sequence of sched_run() calls). The plumbing
// around them is the harness's.

#include "covering.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// The work the critical section protects. Plain (non-atomic) on purpose: if
// mutual exclusion fails, the lost updates show up here as a wrong total.
static long shared_counter = 0;
#define STRESS_ITERS 100000        // K -- TODO (you): pick the contention level

typedef struct { int tid; } arg_t;

// ---- phase 1: stress ------------------------------------------------------
static void *stress_worker(void *p) {
    int tid = ((arg_t *)p)->tid;
    for (int i = 0; i < STRESS_ITERS; i++) {
        cov_lock(tid);
        cs_enter(tid);
        shared_counter++;          // <- the protected critical section
        cs_leave(tid);
        cov_unlock(tid);
    }
    return NULL;
}

static void phase_stress(void) {
    sched_set_mode(SCHED_FREE);
    cov_reset();
    shared_counter = 0;

    pthread_t th[COV_NTHREADS];
    arg_t args[COV_NTHREADS];
    for (int i = 0; i < COV_NTHREADS; i++) {
        args[i].tid = i;
        pthread_create(&th[i], NULL, stress_worker, &args[i]);
    }
    for (int i = 0; i < COV_NTHREADS; i++) pthread_join(th[i], NULL);

    long expected = (long)COV_NTHREADS * STRESS_ITERS;
    printf("[stress] counter=%ld expected=%ld  cs-violations=%d\n",
           shared_counter, expected, cov_violations());

    // TODO (you): write the invariant you actually want to assert, e.g.
    //   assert(shared_counter == expected && cov_violations() == 0);
    // Expect it to PASS most runs once your lock is written -- that's the
    // misleading part the forced phase then demolishes.
}

// ---- phase 2: forced ------------------------------------------------------
// One trip through the lock, for the controller to step instruction by
// instruction.
static void *one_shot(void *p) {
    int tid = ((arg_t *)p)->tid;
    cov_lock(tid);
    cs_enter(tid);
    cs_leave(tid);
    cov_unlock(tid);
    return NULL;
}

static void phase_forced(void) {
    // Until your lock + the schedule below exist, this phase would block
    // (threads park at their first step with nobody driving them). Flip it on
    // with RUN_FORCED=1 once you're ready.
    if (!getenv("RUN_FORCED")) {
        printf("[forced] skipped -- set RUN_FORCED=1 after you write cov_lock + the schedule\n");
        return;
    }

    sched_set_mode(SCHED_DRIVEN);
    cov_reset();

    // In DRIVEN mode the workers block at their first step until sched_run()
    // advances them. tids: A=0, B=1, C=2.
    pthread_t th[COV_NTHREADS];
    arg_t args[COV_NTHREADS];
    for (int i = 0; i < COV_NTHREADS; i++) {
        args[i].tid = i;
        pthread_create(&th[i], NULL, one_shot, &args[i]);
    }

    // TODO (you): drive the covering interleaving, one step per sched_run().
    // Once cov_lock/cov_unlock are written you'll know their exact sequence of
    // loc_read / loc_write / cs_enter steps. The book's shape:
    //
    //   1. Step C (tid 2) through cov_lock and into the CS   -> in_cs = 1.
    //   2. Step A (tid 0) through cov_lock until it is poised to write LA.
    //   3. Step B (tid 1) through cov_lock until it is poised to write LB.
    //   4. Let A write LA, then B write LB. Both locations now look "empty"
    //      again -- every trace that C is inside has been overwritten.
    //   5. Keep stepping B: seeing the locations empty, it walks into the CS
    //      too  -> in_cs = 2  -> cs_enter records a violation. That's the proof.
    //
    // One step looks like:  sched_run(2);  // advance C by one location access
    //
    // NOTE: you must step every thread all the way to the end (through
    // cov_unlock), or the pthread_join below will hang waiting on a thread
    // still parked at a step.

    for (int i = 0; i < COV_NTHREADS; i++) pthread_join(th[i], NULL);
    printf("[forced] cs-violations=%d (want > 0 once the schedule is right)\n",
           cov_violations());
    // TODO (you): assert(cov_violations() > 0);
}

int main(void) {
    printf("== lower bound on locations: the covering argument (book 2.9) ==\n");
    phase_stress();
    phase_forced();
    return 0;
}
