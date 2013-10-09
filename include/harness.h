
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
