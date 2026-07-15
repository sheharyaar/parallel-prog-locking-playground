# Concurrency & Verification — 14-Week Study Plan

**Time budget:** 2 hrs × 5 weekdays + 4 hrs × 1 weekend day = **14 hrs/week**
**Anchor paper:** [Lamport (1977), *Proving the Correctness of Multiprocess Programs*](papers/proving-correctness.pdf)
**Core books:** Ben-Ari *(PCDP)* · Herlihy & Shavit *(TAoMP)* · McKenney *(perfbook)*

---

## Weekly Rhythm (constant every week)

| Day | Slot | Activity |
|---|---|---|
| Mon–Tue | 2 + 2 hrs | Primary reading (textbook chapter) |
| Wed–Thu | 2 + 2 hrs | Second lens on same topic (paper or second book) |
| Fri | 2 hrs | Exercises / Quick Quizzes / notes & journal |
| Weekend | 4 hrs | **Lab** — proofs by hand, coding, or Spin/TLA+ (never new reading) |

> If a week overflows, steal from Friday — never from the weekend lab.

---

## Master Reading List

**Books**
- **PCDP** — Ben-Ari, *Principles of Concurrent and Distributed Programming*, 2e
- **TAoMP** — Herlihy, Shavit, Luchangco, Spear, *The Art of Multiprocessor Programming*, 2e
- **perfbook** — McKenney, *Is Parallel Programming Hard, And, If So, What Can You Do About It?*

**Papers (in order of appearance)**
1. [Lamport (1977) — "Proving the Correctness of Multiprocess Programs"](papers/proving-correctness.pdf) *(the anchor)*
2. Floyd (1967) — "Assigning Meanings to Programs"
3. Hoare (1969) — "An Axiomatic Basis for Computer Programming"
4. Lamport (1974) — "A New Solution of Dijkstra's Concurrent Programming Problem" *(bakery)*
5. Owicki & Gries (1976) — "An Axiomatic Proof Technique for Parallel Programs I"
6. Pnueli (1977) — "The Temporal Logic of Programs" *(skim)*
7. [Alpern & Schneider (1985) — "Defining Liveness"](papers/defending-liveness.pdf)
8. Lamport (1979) — "How to Make a Multiprocessor Computer That Correctly Executes Multiprocess Programs" *(sequential consistency)*
9. Lamport (1978) — "Time, Clocks, and the Ordering of Events" *(optional, Week 13)*
10. Owicki & Lamport (1982) — "Proving Liveness Properties of Concurrent Programs"

**Tools:** Spin/jSpin (Promela), jBACI, herd7 (litmus tests), TLA+ Toolbox

---

# PHASE 1 — The Model & the Critical Section Problem

## ☐ Week 1 — Vocabulary & Setup
- ☐ PCDP Ch. 1–2 (interleaving abstraction, atomicity, correctness, fairness) + Appendix B (logic review)
- ☐ Lamport 1977 §I–II — informal first pass, producer/consumer story only
- ☐ perfbook Ch. 1–2 (light evening reading)
- ☐ **Lab:** install jSpin + jBACI; do the frog-puzzle state diagram (PCDP §2.14) by hand

## ☐ Week 2 — Mutual Exclusion, Attempt by Attempt
- ☐ PCDP Ch. 3 (four failed attempts, Dekker, state-diagram proofs)
- ☐ Papers: Floyd 1967, Hoare 1969
- ☐ TAoMP Ch. 1 + §2.1–2.4 (Peterson lock)
- ☐ **Lab:** prove Peterson correct by hand with invariants; model the "first attempt" in Promela, let Spin find the bug

## ☐ Week 3 — ★ Bakery Week (convergence point)
- ☐ Lamport 1974 (informal bakery proof — read first!)
- ☐ TAoMP §2.6–2.7 (bakery, textbook treatment)
- ☐ PCDP Ch. 5 (N-process bakery, weaker memory models, fast algorithms)
- ☐ Re-read Lamport 1977 **safety half** with full formalism
- ☐ **Lab:** reproduce the producer/consumer safety proof line by line (nₚ, n꜀ trick, monotonicity); write bakery invariants for N=2 yourself

## ☐ Week 4 — Verification Proper
- ☐ PCDP Ch. 4 (temporal logic □/◇, deductive proof of Dekker, Spin/LTL)
- ☐ Owicki–Gries 1976 — map interference freedom ↔ Lamport's monotonicity
- ☐ Lamport 1977 **liveness half** (⤳, axioms L1/L2, Theorem 8); skim Pnueli 1977
- ☐ **Lab:** verify Peterson & Dekker in Spin — mutual exclusion (safety) + eventual entry (liveness with fairness)

---

# PHASE 2 — Primitives & Correctness Conditions

## ☐ Week 5 — Semaphores & Monitors
- ☐ PCDP Ch. 6 (semaphore invariants, producer–consumer, dining philosophers)
- ☐ PCDP Ch. 7 (monitors, readers–writers + correctness proof — the very algorithm Lamport verified in his "Experiment" section)
- ☐ perfbook Ch. 4 (pthreads tools of the trade)
- ☐ **Lab:** implement bounded buffer with pthreads semaphores + condition variables; state and argue the invariant 0 ≤ n ≤ b

## ☐ Week 6 — ★ Linearizability & Progress (TAoMP core)
- ☐ TAoMP Ch. 3 in full: sequential consistency, linearizability, progress zoo (wait-free ⊂ lock-free ⊂ obstruction-free; deadlock-/starvation-free)
- ☐ Alpern & Schneider 1985 (every property = safety ∩ liveness)
- ☐ **Lab:** write the 1-page **Rosetta Stone**: classify every TAoMP concept as safety or liveness; note where fairness enters

## ☐ Week 7 — Hardware Enters
- ☐ perfbook Ch. 3 + Appendix C (caches, MESI, why memory barriers exist)
- ☐ TAoMP Appendix B; revisit PCDP §2.8–2.9 (machine instructions, volatile)
- ☐ **Lab:** perfbook Ch. 5 — code the statistical counter + limit counter; run them; watch the naive one lose counts

---

# PHASE 3 — Real Locks, Real Memory

## ☐ Week 8 — Locks in Practice
- ☐ TAoMP Ch. 7 (TAS/TTAS, backoff, CLH, MCS queue locks)
- ☐ perfbook Ch. 7 (deadlock, livelock, starvation, lock types)
- ☐ **Lab:** implement TAS, TTAS, backoff locks; benchmark under contention; write why the bakery lock is impractical here (O(N) registers, unbounded tickets → TAoMP §2.10 lower bound)

## ☐ Week 9 — Registers & the Power of Primitives
- ☐ TAoMP Ch. 4 (safe/regular/atomic registers — Lamport's own hierarchy; formalizes why the bakery tolerates '?' flickering reads)
- ☐ TAoMP Ch. 5–6 at survey depth (consensus numbers, universality)
- ☐ **Lab:** TAoMP Ch. 4 exercises; sketch why CAS is universal (consensus number ∞)

## ☐ Week 10 — Memory Ordering
- ☐ perfbook Ch. 15 selectively: §15.1–15.3 (intuitions, litmus tests) + your ISA in §15.6
- ☐ Lamport 1979 (sequential consistency — one page, the bridge from the 1977 model to hardware)
- ☐ **Lab:** run litmus tests (herd7 / perfbook examples); break and fix a message-passing pattern with barriers

---

# PHASE 4 — Advanced Structures & Capstone

## ☐ Week 11 — Deferred Processing / RCU
- ☐ perfbook Ch. 9 (refcounting, hazard pointers, seqlocks, RCU fundamentals)
- ☐ TAoMP Ch. 19 (Java→C++, hazard pointers, epochs) as companion
- ☐ **Lab:** perfbook §12.1 — verify toy QRCU in Promela (Week 4 skills applied to Week 11 algorithm)

## ☐ Week 12 — Lock-Free Data Structures
- ☐ TAoMP Ch. 9 (list-based sets: coarse → fine → optimistic → lazy → nonblocking)
- ☐ TAoMP Ch. 10 (Michael–Scott queue, ABA problem)
- ☐ **Lab:** implement lock-free stack or MS queue; annotate each method's **linearization point** in comments

## ☐ Week 13 — Distributed Extension
- ☐ PCDP Ch. 10–12 (Ricart–Agrawala distributed mutex — the bakery's descendant; snapshots; Byzantine consensus)
- ☐ Optional: Lamport 1978 "Time, Clocks…" (bakery timestamps go distributed)
- ☐ **Lab:** start TLA+ — first lectures of Lamport's video course; spec the producer/consumer buffer

## ☐ Week 14 — ★ Consolidation & Capstone
- ☐ Owicki–Lamport 1982 (mature liveness method, proof lattices)
- ☐ **Full re-read of Lamport 1977** — it should now read like a summary of everything you know
- ☐ **Lab (capstone):** spec the bakery algorithm in TLA+ (or Promela); mechanically check mutual exclusion + starvation-freedom; write a 2-page note mapping the mechanical proof to Lamport's hand proof

---

# Deferred List (return when a topic hooks you — none are prerequisites)

**PCDP (Ben-Ari)**
- ☐ Ch. 8 — Channels, rendezvous, RPC
- ☐ Ch. 9 — Spaces (Linda model, master–worker)
- ☐ Ch. 13 — Real-time systems (priority inversion, Mars Pathfinder in Spin, UPPAAL)

**TAoMP (Herlihy & Shavit)**
- ☐ Ch. 8 — Monitors & blocking synchronization *(covered via PCDP Ch. 7; skim for Java specifics)*
- ☐ Ch. 11 — Stacks & elimination
- ☐ Ch. 12 — Counting networks, combining, diffracting trees
- ☐ Ch. 13 — Concurrent hashing (split-ordering, cuckoo)
- ☐ Ch. 14 — Skiplists
- ☐ Ch. 15 — Priority queues
- ☐ Ch. 16 — Scheduling & work stealing
- ☐ Ch. 17 — Data parallelism (MapReduce, streams)
- ☐ Ch. 18 — Barriers
- ☐ Ch. 20 — Transactional programming *(pair with perfbook Ch. 17 if taken)*

**perfbook (McKenney)**
- ☐ Ch. 6 — Partitioning & synchronization design (maze solver)
- ☐ Ch. 8 — Data ownership
- ☐ Ch. 10 — Data structures (RCU hash tables, resizing)
- ☐ Ch. 11 — Validation (heisenbugs, statistics) — *high practical value; good first pick*
- ☐ Ch. 12 §12.2–12.7 — remaining formal verification (PPCMEM, axiomatic, SAT)
- ☐ Ch. 13 — Putting it all together
- ☐ Ch. 14 — Advanced synchronization, NBS, parallel real-time
- ☐ Ch. 16–18 — Ease of use, futures (TM/HTM), retrospective
- ☐ App. B — Toy RCU implementations — *excellent after Week 11*

**Books/papers held in reserve**
- ☐ Apt, de Boer & Olderog, *Verification of Sequential and Concurrent Programs* — only if Phase 1 makes you want full rigor + exercises in Hoare logic / Owicki–Gries
- ☐ Manna & Pnueli, *The Temporal Logic of Reactive and Concurrent Systems* — the deep temporal-logic reference
- ☐ Lamport (1994), "The Temporal Logic of Actions" + *Specifying Systems* — after the Week 14 TLA+ capstone
- ☐ Herlihy & Wing (1990), "Linearizability: A Correctness Condition for Concurrent Objects" — primary source behind TAoMP Ch. 3
- ☐ Ashcroft (1975), Keller (1976) — historical context Lamport argued against
- ☐ Apt (1981), "Ten Years of Hoare's Logic" — compact survey bridge

---

# Standing Habits

1. **Correctness journal** — every algorithm you meet gets an entry: *What's the invariant? Safety or liveness? What fairness is assumed? Where's the linearization point?*
2. Do Quick Quizzes (perfbook) and starred exercises (PCDP/TAoMP) **as you go**, not deferred.
3. Weekend labs are sacred — they're where understanding consolidates.

**Fixed anchor points:** Week 3 (bakery convergence) and Week 6 (linearizability). Everything else can compress or stretch around them.

**Pace check:** review after Week 4. If ahead of schedule, revisit the plan for a revision (compress Phase 2, pull deferred items forward).
