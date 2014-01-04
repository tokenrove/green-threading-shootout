CLANG ?= clang
CFLAGS := $(CFLAGS) -Iinclude -Wall -Wextra -Werror -fstrict-aliasing -Wstrict-aliasing -fms-extensions -std=gnu11 -g -O3
LDFLAGS := $(LDFLAGS) -ltap -lm

# Alternate contestants might include the same ones with different compiler arguments
CONTESTANTS = minimal-stack-swap ucontext
TESTS = simplest_case simple_ab
CLEAN = $(CONTESTANTS:%=t/%.t)

.PHONY: clean all test check check-syntax

all: $(CONTESTANTS:%=t/%.t)

%/..: ; mkdir -p $(@D)

t/%.t: %/*.[cS] tests/harness.c tests/perf_counter_x86_64.S $(TESTS:%=tests/%.c) | t/.. Makefile
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -MD -c $(CFLAGS) -o $@ $<

test: check check-syntax

check: $(CONTESTANTS:%=t/%.t); prove

check-syntax: $(CONTESTANTS:%=%/*.c)
	$(CLANG) -Iinclude --analyze -Wall -Wextra -Werror -pedantic $^
	$(CC) $(CFLAGS) -pedantic -Werror -fsyntax-only $^

clean:
	$(RM) $(CLEAN)
