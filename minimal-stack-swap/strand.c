#include <assert.h>
#include <stdlib.h>
#include "strand.h"

#ifndef NVALGRIND
#include "valgrind.h"
#else
#define VALGRIND_STACK_REGISTER(...)
#endif

struct strand_ {
    void *sp;
    void *parent;
    size_t len;
} strand_;

extern int stack_min_size, stack_alignment;
extern void stack_wrap(strand, int (*)(strand, void *), void *);
extern int stack_swap_to(strand);
extern int stack_swap_from(strand);

strand strand_spawn(int (*fn)(strand, void *), void *data, size_t size_in_words)
{
    struct strand_ *strand;
    size_t len = ((size_in_words * sizeof (void*)) + stack_min_size + (stack_alignment-1)) & (-stack_alignment);
    strand = aligned_alloc(stack_alignment, len);
    (void)VALGRIND_STACK_REGISTER(strand, strand+len);
    strand->sp = strand+len-stack_alignment;
    strand->parent = 0;
    strand->len = len;
    stack_wrap(strand, fn, data);
    return strand;
}

int strand_sched(strand strand)
{
    struct strand_ *s = strand;
    assert(s->sp > strand && s->sp <= (strand+s->len));
    return stack_swap_to(strand);
}

void strand_yield(strand self)
{
    struct strand_ *s = self;
    assert(s->sp > self && s->sp <= (self+s->len));
    stack_swap_from(self);
}
