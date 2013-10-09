
#include "harness.h"

#include <stdio.h>

static void simple_ab()
{
    struct bit { uint64_t *p; bool v; };
    int fn_bit(strand self, void *bit_) {
        struct bit *bit = (struct bit *)bit_;
        for (int i = 0; i < 32; ++i) {
            *bit->p = (*bit->p << 1) | bit->v;
            strand_yield(self);
        }
        return 1;
    }
    strand a, b;
    uint64_t bits = 0;
    bool a_done, b_done;

    note("alternate between two functions");
    a = strand_spawn(fn_bit, &(struct bit){ .p = &bits, .v = 0}, 128);
    b = strand_spawn(fn_bit, &(struct bit){ .p = &bits, .v = 1}, 128);
    do {
        a_done = strand_sched(a);
        b_done = strand_sched(b);
    } while (!a_done and !b_done);
    ok(bits == 0x5555555555555555LL);

    bits = 0;
    a = strand_spawn(fn_bit, &(struct bit){ .p = &bits, .v = 1}, 128);
    b = strand_spawn(fn_bit, &(struct bit){ .p = &bits, .v = 0}, 128);
    do {
        a_done = strand_sched(a);
        b_done = strand_sched(b);
    } while (!a_done and !b_done);
    ok(bits == 0xAAAAAAAAAAAAAAAALL);
}

REGISTER_TEST(simple_ab, 2)
