
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "harness.h"

struct test *tests = NULL;
unsigned int n_tests = 0;

extern uint64_t read_perf_counter(void);

uint64_t determine_perf_counter_overhead(void)
{
    enum { N = 1000 };
    uint64_t v[N], sum = 0;
    for (int i = 0; i < N; ++i) {
        uint64_t a = read_perf_counter();
        uint64_t b = read_perf_counter();
        v[i] = b-a;
        sum += b-a;
    }
    double mean = (double)sum / N, variance = 0.;
    for (int i = 0; i < N; ++i) {
        variance += pow(mean - v[i], 2.);
    }
    double sd = sqrt(variance / (N-1));
    int within = 0;
    for (int i = 0; i < N; ++i) {
        within += abs(v[i] - mean) < 1.96*sd ? 1 : 0;
    }
    diag("performance counter overhead: mean %g, SD %g, %% within: %g\n", mean, sd, 100.*(double)within/N);
    return mean;
}

int main(void)
{
    uint64_t perf_counter_overhead = determine_perf_counter_overhead();

    int expected = 0;
    for (struct test *p = tests; NULL != p; p = p->next) {
        expected += p->expected;
    }
    plan(expected);
    for (struct test *p = tests; NULL != p; p = p->next) {
        diag(p->name);
        uint64_t then = read_perf_counter();
        p->fn();
        uint64_t now = read_perf_counter();
        diag("cycles elapsed: %ld", now - then - perf_counter_overhead);
    }
    done_testing();
}
