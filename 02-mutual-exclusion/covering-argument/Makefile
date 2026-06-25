include ../../common.mk

BIN  := covering
TEST := test_$(BIN)
SRCS := $(BIN).c harness.c          # your lock + the harness plumbing

.PHONY: all test tsan asan clean

all: $(TEST)

$(TEST): test_$(BIN).c $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test: $(TEST)
	./$(TEST)

# A clean TSan run is the real check for anything with threads.
tsan: test_$(BIN).c $(SRCS)
	$(CC) $(CSTD) $(WARN) $(TSAN_FLAGS) $^ -o $(TEST)-tsan $(LDFLAGS)
	./$(TEST)-tsan

asan: test_$(BIN).c $(SRCS)
	$(CC) $(CSTD) $(WARN) $(ASAN_FLAGS) $^ -o $(TEST)-asan $(LDFLAGS)
	./$(TEST)-asan

# No `bench` target here: this lock is broken by design (that's the lesson),
# so throughput numbers would be meaningless.

clean:
	$(RM) $(TEST) $(TEST)-tsan $(TEST)-asan
