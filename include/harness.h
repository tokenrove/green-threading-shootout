
#pragma once

#include <tap.h>
#include <stdint.h>
#include <stdbool.h>
#include <iso646.h>
#include "strand.h"

extern struct test {
    const char *name;
    void (*fn)();
    int expected;
    struct test *next;
} *tests;
extern unsigned int n_tests;

#define REGISTER_TEST(test_name, expected_n_tests)                      \
    static struct test this = {name:#test_name, fn:test_name, expected:expected_n_tests}; \
    static void add_test_to_harness() __attribute__((constructor));     \
    static void add_test_to_harness() { this.next = tests; tests = &this; ++n_tests; }
