# Shared build configuration. Concept Makefiles include this with:
#   include ../../common.mk
CC      ?= gcc
CSTD    ?= -std=c11
WARN    ?= -Wall -Wextra -Wpedantic
OPT     ?= -O2
CFLAGS  ?= $(CSTD) $(WARN) $(OPT) -pthread
LDFLAGS ?= -pthread

# Sanitizer variants — concurrency bugs hide; these find them.
TSAN_FLAGS ?= -fsanitize=thread -g -O1            # data races
ASAN_FLAGS ?= -fsanitize=address,undefined -g -O1 # memory + undefined behavior
