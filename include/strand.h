
#pragma once

#include <stdlib.h>

typedef void *strand;
extern strand strand_spawn(int (*fn)(strand, void *), void *data, size_t size_in_words);
extern int strand_sched(strand strand);
extern void strand_yield(strand self);
