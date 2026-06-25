# The Covering Argument: Why Locks Need n Locations

The Bakery lock works, but it spends one location per thread — `n` threads, `n`
locations. My first reaction was: surely that's just Lamport being unclever, and
some tighter trick gets you a fixed number of locations? §2.9 says no, and the
proof is the most surprising thing in the chapter: a lock built from plain reads
and writes *cannot* do better than `n` locations, ever. This folder is where I
stop nodding along to the proof and actually watch the failure happen on my
machine.

It's also why the rest of the book exists — this lower bound is the reason real
machines ship `compare-and-swap` and friends.

This is a **deep dive**, but a slightly unusual one: you're not building a lock
that works, you're building a lock that *can't*, and a harness that drags the
bug into the open.

## Where this is in the book

- **TAoMP, 2nd ed. — §2.9 "Lower bounds on the number of locations", pp.39–41.**
  The theorem (three threads need three locations) and the covering argument.
- Leans on **§2.1 critical sections** and **§2.4 deadlock-freedom** for the
  definitions, and contrasts with **§2.7 Bakery** (the `n`-location lock this
  proves you can't shrink).

## What you'll build & scope

**You write the lock.** The harness, the shared *locations*, and the scheduler
are scaffolding (`harness.h`/`harness.c`, `test_covering.c`). You fill in
`covering.c` and the two TODO spots in the test (the stress invariant and the
forced-schedule script).

- **Assumes you know:** C and pthreads basics, what a critical section and a data
  race are. No memory-model knowledge needed — the locations are seq-cst atomic
  so a single read or write is one clean step, which is exactly the model the
  proof assumes.
- **Environment:** `gcc` (C11), `-pthread`, Linux, ThreadSanitizer available.
- **The exercise:** implement `cov_lock`/`cov_unlock` for **3 threads** using
  **only the two locations** `LA` and `LB`, reached only through
  `loc_read`/`loc_write`. Then use the harness to show it breaks.

By the end you'll have a candidate 2-location lock, a stress phase where it
*looks* fine, and a forced phase that deterministically catches two threads in
the critical section at once.

## Core concepts explored

Defined on first use; these are what the exercise is really about.

- **Location (read/write register)** — a shared variable reached only by plain
  read and plain write, no read-modify-write. The proof is about how few of
  these a lock can use. Here a location is a `loc_t`.
- **Deadlock-freedom** — if a thread wants in and no one's inside, *some* thread
  eventually gets in. The proof needs only this weak guarantee, which is what
  makes it so damning.
- **The overwrite weakness** — the one fact the whole proof rests on: a value a
  thread writes to a location can be overwritten by another thread *before
  anyone reads it*, leaving no trace it was ever written.
- **Inconsistent state** — the locations read as "nobody is inside and nobody is
  trying," yet a thread really is in the critical section. A correct deadlock-
  free lock can never reach this: a fresh thread would read "all clear" and walk
  straight in, putting two threads inside.
- **Covering state** — a state where some thread is poised to write to *every*
  location while the locations still read "all clear." From here the poised
  threads can fire their writes at once and scrub the memory clean — overwriting
  any trace another thread left while entering. Building one of these is the
  heart of the attack.

## Build & run

The lock body is a stub, so the *forced* phase is gated off until you write it
(`RUN_FORCED=1`). The stress phase runs immediately.

1. **Build.**
   ```sh
   make -C 02-mutual-exclusion/covering-argument
   ```
   Expected: one binary, `test_covering`, no warnings.

2. **Run the stress phase** (forced phase skips itself until you opt in).
   ```sh
   make -C 02-mutual-exclusion/covering-argument test
   ```
   With the empty stub lock you'll see a wrong counter and likely some
   violations even here — no lock at all means no exclusion. Once you've written
   a real candidate, expect the stress line to look *clean* most runs. That's the
   trap the next step springs.

3. **Run the forced phase**, after you've written `cov_lock` and filled in the
   schedule in `test_covering.c`:
   ```sh
   RUN_FORCED=1 make -C 02-mutual-exclusion/covering-argument test
   ```
   Expected once the schedule is right: `[forced] cs-violations=N` with `N > 0`.

4. **ThreadSanitizer** — the usual real check for threaded code:
   ```sh
   make -C 02-mutual-exclusion/covering-argument tsan
   ```
   The locations are atomic, so TSan should stay quiet about *them*; any race it
   reports on `shared_counter` is the broken lock leaking through, which is the
   point.

## Testing & benchmarking notes

The two phases exist to tell one story: the lock *seems* fine until an adversary
picks the schedule. We'll lock down the exact invariant and the step sequence
together — I scaffolded the plumbing (instrumented locations, the cooperative
scheduler, thread spawn/join); the assertion and the script are yours.

1. **Stress — "it looks correct."** 3 threads each do `K` increments of a plain
   shared counter inside the lock; correct iff the total is `3·K`. Run free, this
   will usually pass once your lock is written, because the covering interleaving
   essentially never happens by luck. The lesson is in how convincing this false
   green is — it's the "but a thread *checks* before entering!" intuition.

2. **Forced — "an adversary breaks it."** Same lock, but the controller steps the
   threads one location-access at a time to build a covering state and then scrub
   it: drive C into the critical section, get A and B poised over both locations,
   let them overwrite (erasing C's footprints), then let one of them walk in too.
   `cs_enter` flags the moment two threads are inside. This is the covering
   argument, executed rather than read.

   The harness gates each `loc_read`/`loc_write`/`cs_enter` as one schedulable
   step, so the script is a sequence of `sched_run(tid)` calls. You'll know the
   exact sequence only after your lock is written — its reads and writes are the
   steps. (Gotcha: you must step every thread through to the end, or the join
   hangs on a thread still parked mid-lock.)

No benchmark here: the lock is broken on purpose, so throughput would measure
nothing.

## Online resources

- [Burns & Lynch, "Bounds on shared memory for mutual exclusion" (Information and Computation, 1993)](https://doi.org/10.1006/inco.1993.1062) — the origin of the covering-argument technique the book cites. *(Citation from memory; double-check the page range before quoting it.)*
- [Wikipedia: Mutual exclusion — lower bounds](https://en.wikipedia.org/wiki/Mutual_exclusion) — background and pointers.
- [cppreference: `_Atomic` and `memory_order`](https://en.cppreference.com/w/c/atomic) — the atomics the harness uses for the locations.

## Follow-up questions

Chew on these — I'm not answering them here. They're where the proof goes from
"I followed it" to "I own it."

1. **Why three threads?** The covering state uses A and B to cover the two
   locations. Who is the third thread *for*, and why can't two threads produce
   the contradiction on their own?
2. **Why must every thread write before entering?** Walk the one-line argument:
   what inconsistent state appears if a thread could enter having only read?
3. **The maneuvering.** The book gets A and B into a covering state by running B
   through the critical section three times and using a pigeonhole step. Why
   three trips, and what exactly does pigeonhole buy you with only two locations?
4. **Single-writer vs multi-writer.** For single-writer locations (like Bakery's
   per-thread fields) the bound is immediate; the hard case is multi-writer
   locations (like Peterson's `victim`). Why does multi-writer make it harder,
   and where does your candidate lock sit?
5. **What a third location buys.** If you gave C its own location that A and B
   can't write, why does the covering attack stop working — what trace now
   survives the scrub?
6. **Beating the bound for real.** `compare-and-swap` lets later chapters build
   `n`-thread locks with O(1) locations. Which step of the covering argument does
   an atomic read-modify-write instruction break?
