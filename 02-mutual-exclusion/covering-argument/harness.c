// Harness plumbing for the lower-bound-on-locations demo (book §2.9).
// This file is deliberately generic: it knows nothing about your lock. It just
// provides instrumented shared locations, critical-section accounting, and a
// cooperative scheduler that lets a test force an exact interleaving.

#include "harness.h"
#include <pthread.h>

static sched_mode_t g_mode = SCHED_FREE;
static pthread_mutex_t g_m  = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  g_cv = PTHREAD_COND_INITIALIZER;
static int g_turn = -1;            // worker tid allowed to step; -1 = controller

static atomic_int g_in_cs      = 0;
static atomic_int g_violations  = 0;

void sched_set_mode(sched_mode_t m) {
    pthread_mutex_lock(&g_m);
    g_mode = m;
    g_turn = -1;
    pthread_mutex_unlock(&g_m);
}

// Worker side of a single step. In DRIVEN mode it blocks until the controller
// grants this tid the turn; step_end hands the turn back. In FREE mode both
// are no-ops, so the stress phase runs at full speed.
static void step_begin(int tid) {
    if (g_mode != SCHED_DRIVEN) return;
    pthread_mutex_lock(&g_m);
    while (g_turn != tid) pthread_cond_wait(&g_cv, &g_m);
    pthread_mutex_unlock(&g_m);
}
static void step_end(int tid) {
    if (g_mode != SCHED_DRIVEN) return;
    (void)tid;
    pthread_mutex_lock(&g_m);
    g_turn = -1;
    pthread_cond_broadcast(&g_cv);
    pthread_mutex_unlock(&g_m);
}

int loc_read(int tid, loc_t *L) {
    step_begin(tid);
    int v = atomic_load(&L->v);
    step_end(tid);
    return v;
}

void loc_write(int tid, loc_t *L, int val) {
    step_begin(tid);
    atomic_store(&L->v, val);
    step_end(tid);
}

void cs_enter(int tid) {
    step_begin(tid);
    int n = atomic_fetch_add(&g_in_cs, 1) + 1;
    if (n > 1) atomic_fetch_add(&g_violations, 1);
    step_end(tid);
}

void cs_leave(int tid) {
    step_begin(tid);
    atomic_fetch_sub(&g_in_cs, 1);
    step_end(tid);
}

void sched_run(int tid) {
    pthread_mutex_lock(&g_m);
    g_turn = tid;                      // grant the turn
    pthread_cond_broadcast(&g_cv);
    while (g_turn != -1)               // wait until the worker hands it back
        pthread_cond_wait(&g_cv, &g_m);
    pthread_mutex_unlock(&g_m);
}

int  cov_violations(void) { return atomic_load(&g_violations); }
void cov_reset(void)      { atomic_store(&g_in_cs, 0); atomic_store(&g_violations, 0); }
