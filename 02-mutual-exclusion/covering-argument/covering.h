#ifndef COVERING_H
#define COVERING_H
#include "harness.h"

// Your candidate lock for the book's lower-bound argument (§2.9).
//
// The exercise: build the best mutual-exclusion lock you can for THREE threads
// using only TWO shared locations. The covering argument says you can't -- the
// harness will help you watch it fail.

#define COV_NTHREADS 3   // A=0, B=1, C=2  (the proof's three-thread case)

// The two -- and only two -- shared locations your lock may use. Defined in
// covering.c, both starting "empty" (LOC_EMPTY). Adding a third location
// defeats the point of the exercise.
extern loc_t LA, LB;

// Acquire / release the lock for worker `tid` (0..COV_NTHREADS-1).
// Build these out of loc_read(tid, &LA/&LB) and loc_write(tid, &LA/&LB, ...)
// ONLY -- no other shared state, no atomics of your own.
void cov_lock(int tid);
void cov_unlock(int tid);

#endif
