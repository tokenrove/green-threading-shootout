
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "harness.h"

struct test *tests = NULL;
unsigned int n_tests = 0;

int main(void)
{
    int expected = 0;
    for (struct test *p = tests; NULL != p; p = p->next) {
        expected += p->expected;
    }
    plan(expected);
    for (struct test *p = tests; NULL != p; p = p->next) {
        note(p->name);
        p->fn();
    }
    done_testing();
}
