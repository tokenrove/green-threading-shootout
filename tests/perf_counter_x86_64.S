
        .text
        .global read_perf_counter
read_perf_counter:
        rdtscp
        shl $32, %rdx
        or %rdx, %rax
        ret
