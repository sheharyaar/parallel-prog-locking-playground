#ifndef BAKERY_H
#define BAKERY_H

/*
 * Lamport's Bakery lock for a FIXED set of N threads.
 *
 * Each thread has a stable id in [0, n). It passes that id to lock()/unlock()
 * so the lock can tell callers apart. (That a thread must identify itself is
 * inherent to an n-thread read/write lock — it is NOT the trick. The trick is
 * what each thread publishes and how they order themselves. That part is in
 * bakery.c, and it's yours to design.)
 *
 * This header is the contract the tests and benchmark compile against. Keep the
 * signatures; the internal representation of `bakery_lock` is up to you.
 */

typedef struct bakery_lock bakery_lock_t;   /* opaque; you define it in bakery.c */

/* Create a lock for `n` threads. Returns NULL on failure. */
bakery_lock_t *bakery_create(int n);

/* Free a lock from bakery_create. Safe to call with NULL. */
void bakery_destroy(bakery_lock_t *lock);

/* Acquire / release the lock on behalf of thread `tid`, 0 <= tid < n. */
void bakery_lock(bakery_lock_t *lock, int tid);
void bakery_unlock(bakery_lock_t *lock, int tid);

#endif /* BAKERY_H */
