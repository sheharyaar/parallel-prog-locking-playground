# Parallel Programming & Locking Playground

I'm working through *The Art of Multiprocessor Programming* (Herlihy, Shavit,
Luchangco, Spear) and the only way I actually understand a lock or a lock-free
structure is to build it myself. So this repo is my lab: pick a concept from the
book, implement it **in C**, then write tests and benchmarks that make its
behavior (and its bugs) visible.

Each concept folder is meant to read as a standalone tutorial - the plan is to
export the tree as a blog series later. If you're a reader, the per-concept
READMEs are the real content; this page is just the map.

## Prerequisites

- `gcc` (C11) and GNU `make`
- A machine with a few cores so contention is real
- The book handy for chapter references

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
  chapter level (chapter folders are just containers).
- Build config is shared via `common.mk`; the root `Makefile` recurses.
- `AGENT.md` describes how the study agent assists here — chiefly: it scaffolds
  tooling and writes docs, but it never implements the concepts. That part's
  mine.
