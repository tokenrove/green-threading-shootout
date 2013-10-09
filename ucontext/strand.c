
#include "strand.h"
#include <ucontext.h>

struct strand {
    ucontext_t context;
    ucontext_t parent;
};

static int return_code = 0;

static void strand_wrap(strand self, void *data, int (*fn)(strand, void*))
{
    return_code = (*fn)(self, data);
}

strand strand_spawn(int (*fn)(strand, void *), void *data, size_t size)
{
    struct strand *strand = malloc(sizeof (*strand));
    getcontext(&strand->parent);
    getcontext(&strand->context);
    strand->context.uc_link = &strand->parent;
    posix_memalign(&strand->context.uc_stack.ss_sp, 16, size * sizeof (void*));
    strand->context.uc_stack.ss_size = size;
    makecontext(&strand->context, (void(*)(void))strand_wrap, 3, strand, data, fn);
    return strand;
}

int strand_sched(strand strand_)
{
    struct strand *strand = strand_;
    return_code = 0;
    swapcontext(&strand->parent, &strand->context);
    return return_code;
}


void strand_yield(strand self_)
{
    struct strand *self = self_;
    swapcontext(&self->context, &self->parent);
}
