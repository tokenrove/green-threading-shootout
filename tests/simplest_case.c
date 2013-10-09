
#include "harness.h"

#include <stdio.h>

static void simplest_case()
{
    int just_return(strand self __attribute__((unused)), void *_ __attribute__((unused))) { return 42; }
    int yield_once(strand self, void *_ __attribute__((unused))) { strand_yield(self); return -1; }
    strand p;
    p = strand_spawn(just_return, NULL, 128);
    cmp_ok(strand_sched(p), "==", 42);
    p = strand_spawn(yield_once, NULL, 128);
    cmp_ok(strand_sched(p), "==", 0);
    cmp_ok(strand_sched(p), "==", -1);
}

REGISTER_TEST(simplest_case, 3)
