# Parallel Programming & Locking Playground

I'm working through a self-made 14-week plan on concurrency and its
verification, built around three books - Ben-Ari's *Principles of Concurrent and
Distributed Programming*, Herlihy & Shavit's *The Art of Multiprocessor
Programming*, and McKenney's *Is Parallel Programming Hard…* - plus the papers
that started it all (Lamport, mostly). The only way I actually understand a lock,
a lock-free structure, or a correctness proof is to make it myself. So this repo
is my lab: take a concept, implement it **in C**, or model it in **Promela/Spin**
or **TLA+**, then write the tests, benchmarks, and checks that make its behavior
(and its bugs) visible.

Each concept folder is meant to read as a standalone tutorial - the plan is to
export the tree as a blog series later. If you're a reader, the per-concept
READMEs are the real content; this page is just the map.

## The plan & the resources

The week-by-week roadmap lives in
[`resources/concurrency-study-plan.md`](resources/concurrency-study-plan.md) -
reading order, the paper spine, and the weekend lab that turns each topic into
something built or checked. `resources/` also holds the books (PDF) and the two
verification tools I lean on: [`jspin`](resources/jspin), a front-end for the
[Spin](https://spinroot.com) model checker, and [`jbaci`](resources/jbaci), the
BACI concurrency simulator - both pulled in as git submodules.

Two fixed anchor points in the plan: **Week 3 (Bakery)** and **Week 6
(linearizability)**. Everything else stretches or compresses around them.

## Prerequisites

- `gcc` (C11) and GNU `make` - for the C implementations
- A machine with a few cores so contention is real
- For the verification labs (optional, per concept): a JVM to run
  [`jspin`](resources/jspin) / [`jbaci`](resources/jbaci), the
  [Spin](https://spinroot.com) model checker, and eventually TLA+
- The books handy for chapter references (they live in `resources/`)
- `git lfs` installed and `git submodule update --init` run, to pull the book
  PDFs and the tool submodules

## Build steps

```sh
make                 # build every concept
make -C 07-spin-locks-and-contention/clh-queue-lock        # build one concept
make -C <concept-dir> test    # run its test harness
make -C <concept-dir> bench   # run its benchmark
make -C <concept-dir> tsan    # rebuild under ThreadSanitizer (the real check)
make list            # list all concepts
make clean           # clean everything
```

For anything with synchronization, a clean `tsan` run is the green light — a
plain build passing means little for a lock.

## Index

Grouped by book chapter. Status: 🚧 wip · ✅ done · 💤 idea.

| Chapter | Concept | Status | Link |
|---|---|---|---|
| 2 — Mutual exclusion | Lamport's Bakery algorithm | 🚧 wip | [bakery-lock](02-mutual-exclusion/bakery-lock) |

More rows as concepts get built.

## How this repo is organized

- Layout is two levels: `NN-chapter-slug/concept-slug/`.
- A `Makefile` and `README.md` live in each **concept** folder — never at the
  chapter level (chapter folders are just containers). A concept folder holds C,
  or a Promela/TLA+ model, or a hand-proof note — whatever the lab calls for.
- Build config is shared via `common.mk`; the root `Makefile` recurses.
- `resources/` is read-only input: the books, the study plan, and the tool
  submodules. Nothing there gets edited.
- `main` is kept **exercise-only** — scaffolding with `TODO` markers, no
  solutions — so the tree stays clean for the eventual blog export. I implement
  each concept on a separate branch.
- `AGENT.md` describes how the study agent assists here — chiefly: it scaffolds
  tooling and writes docs, but it never implements the concepts or writes the
  proofs. That part's mine.
