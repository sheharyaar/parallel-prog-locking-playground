# Lamport's Bakery Algorithm

I used to treat `lock()` as magic: call it, you're in, somebody else's problem.
What pulled me into the Bakery algorithm is that it builds a *fair*, n-thread
lock out of nothing but ordinary reads and writes to shared memory — no
compare-and-swap, no test-and-set, no special instruction. Just threads
scribbling numbers where everyone can see them, like taking a paper ticket at a
deli counter: lowest number goes next. The question that kept me up: how do
threads agree on an order when the only thing they can do is read and write?

This is a **deep dive**. The goal isn't just a lock that works — it's to come
out understanding *why* it gives first-come-first-served fairness and where it
quietly cheats (it does).

## Where this is in the book

- **TAoMP, 2nd ed. — §2.7 "Lamport's Bakery algorithm", p.34.** The algorithm
  itself.
- **§2.8 "Bounded timestamps", p.35** — picks up the loose end Bakery leaves
  (its ticket numbers grow without bound). Read it after, not before.
- Background from earlier in the chapter you'll lean on: **§2.1 critical
  sections**, **§2.6 fairness**, and the two-thread Peterson lock (§2.3.3) for
  contrast.

## What you'll build & scope

**You write the algorithm.** The scaffolding here is the public API
(`bakery.h`), a skeleton `bakery.c` with `TODO (you)` markers, a Makefile, and
(once we co-design it) the test harness. The lock logic is yours.

- **Assumes you know:** C and pthreads basics, what a critical section is, and
  what a data race is. You don't need the C11 memory model yet — Stage 1 leans
  on the strongest, simplest ordering.
- **Environment:** `gcc` (C11), `-pthread`, Linux, ThreadSanitizer available.
- **Stage 1 (this exercise):** a correct Bakery lock for a fixed `n` threads,
  using C11 `_Atomic` with the default **sequentially consistent** ordering
  (defined below). Target properties: mutual exclusion + FCFS.
- **Stage 2 (a later follow-up):** relax the ordering and watch the memory model
  bite. Flagged in the questions at the end; not now.

By the end of Stage 1 you'll have `bakery.c` implementing `bakery.h`, a test
that *loses increments* when the lock is broken, and a clean ThreadSanitizer
run.

## Core concepts explored

Defined on first use; these are the ideas this exercise is really about.

- **Critical section** — a region of code that at most one thread may execute at
  a time.
- **Mutual exclusion** — the guarantee that no two threads are in the critical
  section simultaneously. The minimum bar for a lock.
- **Deadlock-freedom** — if some thread wants in, *some* thread eventually gets
  in (the system doesn't freeze). Weaker than:
- **Starvation-freedom / FCFS (first-come-first-served)** — *every* thread that
  wants in eventually gets in, and in Bakery's case, in roughly the order they
  asked. This fairness is the whole point of the algorithm.
- **Doorway section** — the opening part of `lock()` that completes in a bounded
  number of steps no matter what other threads do (it can't spin). Bakery splits
  `lock()` into a doorway followed by a wait; the split is what makes FCFS
  precise.
- **Read/write register** — a shared variable supporting only plain reads and
  writes (no atomic read-modify-write). Bakery uses *only* these — that's its
  claim to fame.
- **Sequential consistency** — the memory model where all threads see one single
  interleaving of operations, consistent with each thread's own program order.
  It's what the textbook silently assumes and what Stage 1 buys with seq_cst
  atomics. Real hardware doesn't give it for free — hence Stage 2.

## Build & run

You won't be able to build until `bakery.c` has real bodies and the test harness
exists (we co-design that next — see below). Once both are in place:

1. **Build the test and benchmark.**
   ```sh
   make -C 02-mutual-exclusion/bakery-lock
   ```
   Expected: two binaries, `test_bakery` and `bench_bakery`, no warnings.
   _(Fill in exact output once it builds.)_

2. **Run the correctness test.**
   ```sh
   make -C 02-mutual-exclusion/bakery-lock test
   ```
   Expected: a PASS line and the final counter equal to `threads × iterations`.
   _(Fill in once implemented.)_

3. **Run it under ThreadSanitizer — this is the real check.**
   ```sh
   make -C 02-mutual-exclusion/bakery-lock tsan
   ```
   Expected: `ThreadSanitizer: reported 0 warnings`. For a lock, a clean plain
   build means little; a clean TSan run is the green light.
   _(Fill in once implemented.)_

   Common error: if TSan flags a race on the shared counter, the lock isn't
   actually excluding — the bug is in `bakery.c`, not the test.

## Testing & benchmarking notes

Two properties are worth testing, and they're different in difficulty. These are
the proposed workloads — we'll lock the exact invariant down together before any
harness code gets written (that's the deal: I scaffold the thread plumbing, you
write the assertion).

1. **Mutual exclusion — "the counter doesn't lie."** Spawn `n` threads; each does
   `K` increments of a single **non-atomic** shared counter, *inside* the lock.
   Correct iff the final value is exactly `n × K`. A lock that ever lets two
   threads in at once loses increments to the lost-update race. The counter is
   deliberately non-atomic so the lock is the *only* thing protecting it — a
   sharper test than an atomic counter, which would paper over a broken lock.
   Run it under TSan: a correct lock yields zero races on the counter.

2. **FCFS / fairness — "did they enter in line order?"** Harder to observe: you
   record the order threads finish the doorway versus the order they enter the
   critical section, and check they match (within the algorithm's guarantee).
   This one needs care to instrument without the instrumentation itself changing
   the ordering. We'll design it after Stage 1's mutual-exclusion test is green.

3. **Benchmark — throughput vs thread count.** Sweep `1, 2, 4, 8, …` threads and
   measure lock acquisitions per second. Expect Bakery to scale *badly*: each
   `lock()` reads all `n` of the other threads' state (O(n) work per acquire) and
   needs O(n) space. The benchmark should make that cost visible — it's a big
   reason real systems don't use it.

## Online resources

- [Leslie Lamport, "A New Solution of Dijkstra's Concurrent Programming Problem" (CACM, 1974)](https://lamport.azurewebsites.net/pubs/pubs.html#bakery) — the original paper that introduced the Bakery algorithm.
- [Wikipedia: Lamport's bakery algorithm](https://en.wikipedia.org/wiki/Lamport%27s_bakery_algorithm) — clear pseudocode and a worked intuition.
- [cppreference: `_Atomic` and `memory_order`](https://en.cppreference.com/w/c/atomic) — the C11 atomics you'll use for Stage 1 (and relax in Stage 2).
- [Preshing, "Memory Barriers Are Like Source Control Operations"](https://preshing.com/20120710/memory-barriers-are-like-source-control-operations/) — save this for Stage 2, when seq_cst comes off.

## Follow-up questions

Chew on these — I'm not going to answer them for you. They're the difference
between "I typed in a lock" and "I understand the Bakery."

1. **The doorway.** Why must a thread publish its intent *before* it reads
   everyone else's numbers, not after? What goes wrong if you swap the order?
2. **Ties.** Two threads can read the current maximum at the same instant and
   pick the *same* ticket number. Why doesn't that break mutual exclusion — what
   breaks the tie between them?
3. **Cost of fairness.** Peterson (§2.3.3) gives mutual exclusion for 2 threads.
   What does Bakery give you that "Peterson, but for n threads" wouldn't — and
   what do you pay for it, per acquire and in space?
4. **Unbounded numbers.** Ticket numbers only ever grow. On a real machine an
   integer eventually overflows. What *specifically* breaks when a number wraps,
   and how does §2.8 sidestep it without infinite integers?
5. **Stage 2 teaser.** If you drop one of your atomics from seq_cst to
   `memory_order_relaxed`, which guarantee of the algorithm breaks *first*?
   Predict it before you measure it.
6. **Test design.** Why is a *non-atomic* shared counter a sharper mutual-
   exclusion test than an atomic one?
