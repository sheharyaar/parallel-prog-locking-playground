# Mission: Understanding concurrency by building it (The Art of Multiprocessor Programming)

## Why
Reading about a lock isn't the same as making one work under contention. I'm
building selected concurrency primitives from *The Art of Multiprocessor
Programming* by hand, in C, so I can actually reason about and write correct
concurrent code — and turn the journey into a tutorial blog series others can
learn from.

## Success looks like
- I implement selected concepts from the book in C, and they run clean under
  ThreadSanitizer (a passing plain build isn't enough for a lock).
- I can explain each concept in my own words — captured in its concept README
  and in the follow-up questions I've worked through.
- The repo exports cleanly as a coherent tutorial blog series, useful to a
  reader who wasn't here while I built it.

## Constraints
- Language is **C** (gcc, C11, pthreads, C11 atomics). Self-paced, chapter by
  chapter, picking concepts — not marching the whole book.
- **Hands-on first:** the agent scaffolds tooling, docs, and test plumbing, but
  never writes the algorithm or the answers. The learning is in my fingers.
- Tests and benchmark workloads are co-designed, not handed over finished — the
  point is to understand the workload, not just pass it.

## Out of scope
- Java/C++ ports of the book's code, and exhaustive coverage of every concept.
- Formal correctness proofs for their own sake (I care about the intuition and
  the working code, not the full proof apparatus).
- Agent-delivered "explainers" that walk through an implementation before I've
  built it — that would undercut the whole point.
