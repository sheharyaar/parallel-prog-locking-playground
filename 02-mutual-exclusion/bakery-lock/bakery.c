#include "bakery.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

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
    _Atomic(bool) *flags;
    _Atomic(unsigned int) *labels;
};

bakery_lock_t *bakery_create(int n) {
	bakery_lock_t *lock = malloc(sizeof(*lock));
	if(!lock) return NULL;

	lock->n = n;
	lock->flags = calloc(n, sizeof(bool));
	lock->labels = calloc(n, sizeof(unsigned int));
	
	if (!lock->flags || !lock->labels) {
		bakery_destroy(lock);
		return NULL;
	}

	return lock;
}

void bakery_destroy(bakery_lock_t *lock) {
	if (!lock) return;

	if(lock->flags) free((void *)lock->flags);
	if(lock->labels) free((void *)lock->labels);
	lock->flags = NULL;
	lock->labels = NULL;
	free(lock);
}

static inline bool label_comp(unsigned int label_i, int i, unsigned int label_j, int j) {
	if (label_i == label_j) return (i<j);
	return (label_i < label_j);
}

void bakery_lock(bakery_lock_t *lock, int tid) {
	if (tid >= lock->n) return;

	lock->flags[tid] = true;
	unsigned int max_label=0;
	for (int i =0; i < lock->n; i++) {
		if (lock->labels[i] > max_label)
			max_label = lock->labels[i];
	}

	lock->labels[tid] = max_label + 1;

	for (int i=0; i < lock->n; i++) {
		if (i == tid) continue;

		while (lock->flags[i] && label_comp(lock->labels[i], i, lock->labels[tid], tid)) {
		};
	}
}

void bakery_unlock(bakery_lock_t *lock, int tid) {
	if (tid >= lock->n) return;
	lock->flags[tid] = false;
}
