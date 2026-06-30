include ../../common.mk

BIN   := bakery
TEST  := test_$(BIN)
BENCH := bench_$(BIN)

# Stage 2: the same lock with its memory ordering relaxed, plus a multi-trial
# test that makes the breakage show up. Opt-in (not part of `all`), like the
# covering-argument forced phase.
RELAXED      := bakery_relaxed
TEST_RELAXED := test_relaxed

.PHONY: all test bench tsan asan relaxed relaxed-tsan clean

all: $(TEST) $(BENCH)

$(TEST): test_$(BIN).c $(BIN).c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BENCH): bench_$(BIN).c $(BIN).c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test: $(TEST)
	./$(TEST)

bench: $(BENCH)
	./$(BENCH)

tsan: test_$(BIN).c $(BIN).c
	$(CC) $(CSTD) $(WARN) $(TSAN_FLAGS) $^ -o $(TEST)-tsan $(LDFLAGS)
	./$(TEST)-tsan

asan: test_$(BIN).c $(BIN).c
	$(CC) $(CSTD) $(WARN) $(ASAN_FLAGS) $^ -o $(TEST)-asan $(LDFLAGS)
	./$(TEST)-asan

# Stage 2: build + run the relaxed lock against the multi-trial test.
relaxed: $(TEST_RELAXED).c $(RELAXED).c
	$(CC) $(CFLAGS) $^ -o $(TEST_RELAXED) $(LDFLAGS)
	./$(TEST_RELAXED)

# The real check for Stage 2: TSan should flag the race the relaxation opens.
relaxed-tsan: $(TEST_RELAXED).c $(RELAXED).c
	$(CC) $(CSTD) $(WARN) $(TSAN_FLAGS) $^ -o $(TEST_RELAXED)-tsan $(LDFLAGS)
	./$(TEST_RELAXED)-tsan

clean:
	$(RM) $(TEST) $(BENCH) $(TEST)-tsan $(TEST)-asan
	$(RM) $(TEST_RELAXED) $(TEST_RELAXED)-tsan
