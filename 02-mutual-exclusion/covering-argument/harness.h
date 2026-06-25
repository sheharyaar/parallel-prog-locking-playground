#ifndef COV_HARNESS_H
#define COV_HARNESS_H
// Test harness for the "lower bound on the number of locations" demo (book
// §2.9). The harness owns the *shared locations* and the critical-section
// bookkeeping, so it can (a) detect when mutual exclusion breaks and (b) drive
// an exact thread interleaving. Your lock (covering.c) is built ON TOP of these.

#include <stdatomic.h>

#define LOC_EMPTY 0   // the "bottom"/nobody value the book writes as ⊥

// A shared memory location: a single value reached only by read and write.
// It's atomic so that one lone read or one lone write is a single well-defined
// step -- exactly the model the covering argument assumes. The *lock logic*
// you layer on top is what may be wrong, not the location itself.
typedef struct { _Atomic int v; } loc_t;

// Read / write a location. `tid` is the calling worker's id (0-based).
// In driven mode (see below) each of these is one schedulable step.
int  loc_read (int tid, loc_t *L);
void loc_write(int tid, loc_t *L, int val);

// Mark the critical-section boundary. cs_enter/cs_leave track how many threads
// are inside at once; if that ever exceeds one, mutual exclusion is broken --
// the bug this whole concept is built to expose.
void cs_enter(int tid);
void cs_leave(int tid);

// ---- deterministic scheduler (for the forced-schedule phase) --------------
typedef enum { SCHED_FREE, SCHED_DRIVEN } sched_mode_t;

// FREE: workers run unsynchronised (the stress phase).
// DRIVEN: workers block at every step until the controller advances them.
// Only switch modes when no worker is mid-step (i.e. between phases).
void sched_set_mode(sched_mode_t m);

// Controller side (call from the main/test thread in DRIVEN mode): let worker
// `tid` take exactly one step -- one loc_read/loc_write or one cs_enter/
// cs_leave -- and block until that step finishes.
void sched_run(int tid);

// ---- violation reporting --------------------------------------------------
int  cov_violations(void);   // times more than one thread was in the CS at once
void cov_reset(void);        // zero the counters (call between phases)

#endif
