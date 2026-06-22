# Root build. Discovers every concept (a second-level dir with a Makefile)
# and forwards targets to it. No per-chapter logic — chapters are containers.
CONCEPTS := $(dir $(wildcard */*/Makefile))

.PHONY: all clean list $(CONCEPTS)

all: $(CONCEPTS)

$(CONCEPTS):
	$(MAKE) -C $@

clean:
	@for d in $(CONCEPTS); do $(MAKE) -C $$d clean; done

list:
	@echo "Concepts:"; for d in $(CONCEPTS); do echo "  $$d"; done
