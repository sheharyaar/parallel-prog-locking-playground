# Bakery Lock — Solutions & Notes

> **Template only on `main`.** The questions and section headings live here so
> the structure is shared, but the *answers* get filled in on the `solutions`
> branch (keep `main` spoiler-free — it's the exercise). If you're reading this
> on `main` and the answers are blank, that's intentional.

## Stage 1 follow-up questions

### 1. The doorway
*Why must a thread publish its intent before it reads everyone else's numbers,
not after? What goes wrong if you swap the order?*

_(your answer)_

### 2. Ties
*Two threads can read the current maximum at the same instant and pick the same
ticket number. Why doesn't that break mutual exclusion — what breaks the tie?*

_(your answer)_

### 3. Cost of fairness
*What does Bakery give you that "Peterson, but for n threads" wouldn't — and what
do you pay for it, per acquire and in space?*

_(your answer)_

### 4. Unbounded numbers
*Ticket numbers only ever grow. What specifically breaks when a number wraps, and
how does §2.8 sidestep it without infinite integers?*

_(your answer)_

### 5. Stage 2 teaser
*If you drop one of your atomics from seq_cst to `memory_order_relaxed`, which
guarantee of the algorithm breaks first? Predict it before you measure it.*

_(your prediction — write this BEFORE running Stage 2)_

### 6. Test design
*Why is a non-atomic shared counter a sharper mutual-exclusion test than an
atomic one?*

_(your answer)_

## Stage 2 — what actually broke

Fill this in from the `relaxed` / `relaxed-tsan` runs.

- **Operation(s) relaxed (and to what order):** _(e.g. the doorway flag store, seq_cst → relaxed)_
- **Property predicted to break first:** _(from Q5)_
- **What actually broke, and was the prediction right:** _(your answer)_
- **`make relaxed` result:** _(e.g. failed=N/1000, worst deficit=…)_
- **`make relaxed-tsan` report:** _(which access pair TSan flagged)_
- **Why that ordering was load-bearing:** _(the one-line memory-model argument)_
