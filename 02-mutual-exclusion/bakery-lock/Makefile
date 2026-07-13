include ../../common.mk

BIN   := bakery
TEST  := test_$(BIN)
BENCH := bench_$(BIN)

.PHONY: all test bench tsan asan clean

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

clean:
	$(RM) $(TEST) $(BENCH) $(TEST)-tsan $(TEST)-asan
