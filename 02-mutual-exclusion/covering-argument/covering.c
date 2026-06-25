#include "covering.h"

// The two shared locations. Both start "empty" (the book's ⊥).
loc_t LA = { LOC_EMPTY };
loc_t LB = { LOC_EMPTY };

// TODO (you): implement a candidate 2-location lock for 3 threads.
//
// Rules of the exercise (from §2.9):
//   - Touch ONLY LA and LB, and only through loc_read / loc_write.
//   - Every thread must write at least one location before entering the CS.
//     (A thread that enters without writing leaves memory looking "empty"
//      while it's inside -- an inconsistent state, see the README.)
//   - Aim for deadlock-freedom: a thread that sees the locations empty must
//     be able to get in.
//
// You will not manage to make it correct -- that's the theorem. Use the
// harness's forced phase to drive the covering interleaving and catch two
// threads in the critical section.

void cov_lock(int tid) {
    (void)tid;
    // TODO (you): spin until it's safe to enter, using LA and LB only.
}

void cov_unlock(int tid) {
    (void)tid;
    // TODO (you): release, using LA and LB only.
}
