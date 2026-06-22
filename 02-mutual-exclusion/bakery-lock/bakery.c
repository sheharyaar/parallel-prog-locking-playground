#include "bakery.h"
#include <stdlib.h>

/*
 * ======================================================================
 *  THE ALGORITHM IS YOURS TO WRITE.  (AGENT.md §0)
 * ======================================================================
 *
 * This is a skeleton: lifecycle plumbing + empty bodies. Everything that makes
 * it a *Bakery* lock — the state each thread publishes, the doorway, the wait
 * condition — you fill in. Reference: TAoMP 2nd ed., §2.7 (p.34).
 *
 * Stage 1 target (this exercise): correctness under sequential consistency.
 * Use C11 atomics (<stdatomic.h>, _Atomic) with the DEFAULT memory order
 * (memory_order_seq_cst). Get mutual exclusion and FCFS right before you think
 * about weaker orderings — that's Stage 2 (see the README follow-ups).
 *
 * Questions to design against (don't answer them here, answer them in code):
 *   - What does each thread need to make visible so others can queue behind it?
 *   - What is the smallest "doorway" — the part that finishes in bounded steps?
 *   - Two threads pick the same number. What breaks the tie?
 *
 * If you'd rather start from a blank file, delete this and write your own.
 */

struct bakery_lock {
    int n;
    /* TODO (you): the state that makes this a Bakery lock. */
};

bakery_lock_t *bakery_create(int n) {
    (void)n;
    return NULL;   /* TODO (you): allocate + initialize. */
}

void bakery_destroy(bakery_lock_t *lock) {
    (void)lock;    /* TODO (you): free whatever bakery_create allocated. */
}

void bakery_lock(bakery_lock_t *lock, int tid) {
    (void)lock; (void)tid;   /* TODO (you): doorway, then wait. */
}

void bakery_unlock(bakery_lock_t *lock, int tid) {
    (void)lock; (void)tid;   /* TODO (you): step out of line. */
}
