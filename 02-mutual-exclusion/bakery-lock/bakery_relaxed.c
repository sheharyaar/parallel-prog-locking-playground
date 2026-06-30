#include "bakery.h"
#include <stdlib.h>

/*
 * ======================================================================
 *  STAGE 2 — THE ALGORITHM (AND ITS ORDERING) IS YOURS.  (AGENT.md §0)
 * ======================================================================
 *
 * Stage 1 bought sequential consistency with seq_cst atomics, and the lock
 * worked. This file is the *same Bakery lock* with the memory ordering
 * deliberately weakened — the exercise is to break it on purpose and watch a
 * real memory model bite. Reference: TAoMP 2nd ed. §2.7 (the lock) read
 * against the C11 model (<stdatomic.h>, memory_order_*).
 *
 * It implements the SAME contract as bakery.h, so it links into its own test
 * binary (test_relaxed) without touching your Stage 1 bakery.c.
 *
 * The exercise:
 *   1. Bring over your Stage 1 lock body.
 *   2. Drop the ordering on the operations from memory_order_seq_cst toward
 *      memory_order_relaxed (or acquire/release). Change as little as possible
 *      — ideally ONE operation at a time — so the failure is attributable.
 *   3. PREDICT which property breaks first (mutual exclusion? FCFS?) before you
 *      run test_relaxed. Then run it and see if the counter agrees.
 *
 * Questions to design against (don't answer them here — answer them in
 * SOLUTION.md, and in which ordering you choose):
 *   - The doorway publishes "I'm picking a number" and then the scan reads
 *     everyone else. Which single ordering, when relaxed, lets a peer read a
 *     stale "not in the doorway" and walk in alongside you?
 *   - Which relaxations are still safe on x86's TSO, and which break even
 *     there? (The store->load reorder is the one TSO already allows.)
 *   - A clean plain build proving "it works" means nothing here. What is the
 *     real check that the relaxation introduced a race?  (Hint: `make
 *     relaxed-tsan`.)
 *
 * If you'd rather start from a blank file, delete this and write your own.
 */

struct bakery_lock {
    int n;
    /* TODO (you): the same state as your Stage 1 lock — but you'll annotate the
     * accesses below with weaker memory orders. */
};

bakery_lock_t *bakery_create(int n) {
    (void)n;
    return NULL;   /* TODO (you): allocate + initialize (same as Stage 1). */
}

void bakery_destroy(bakery_lock_t *lock) {
    (void)lock;    /* TODO (you): free whatever bakery_create allocated. */
}

void bakery_lock(bakery_lock_t *lock, int tid) {
    (void)lock; (void)tid;   /* TODO (you): your doorway + wait, with ordering
                              * relaxed on the operation(s) you're testing. */
}

void bakery_unlock(bakery_lock_t *lock, int tid) {
    (void)lock; (void)tid;   /* TODO (you): step out of line. */
}
