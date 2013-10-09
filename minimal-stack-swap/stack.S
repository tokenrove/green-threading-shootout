/*
 *
 */

              .macro push_caller_save_regs
              push %rbp
              push %rbx
              push %r12
              push %r13
              push %r14
              push %r15
              .endm

              .macro pop_caller_save_regs
              pop %r15
              pop %r14
              pop %r13
              pop %r12
              pop %rbx
              pop %rbp
              .endm

              .text
              .global stack_wrap
              // stack_wrap(strand, fn, data) -- prep strand's stack
              // for being swapped.
stack_wrap:
              // initialize parent, sp
              push %rbp
              mov %rsp, %rbp
              mov %rbp, 8(%rdi)
              mov 16(%rdi), %rax
              add %rdi, %rax
              mov %rax, 0(%rdi)
              // swap the stacks
              mov %rax, %rsp
              push %rsi
              push %rdx
              pushq $1f
              push_caller_save_regs
              mov %rsp, 0(%rdi)
              mov %rbp, %rsp
              pop %rbp
              ret
1:            pop %rsi
              pop %rdx
              mov %rdi, %rbx
              call *%rdx
              // mark stack as deallocatable here
              movq $0, 0(%rbx)
              mov 8(%rbx), %rsp
              pop_caller_save_regs
              // return value is in rax already
              ret   

              .align 16
              .global stack_swap_from
stack_swap_from:
              push_caller_save_regs
              mov %rsp, 0(%rdi)
              mov 8(%rdi), %rsp
              pop_caller_save_regs
              xor %eax, %eax
              ret

              .align 16
              .global stack_swap_to
stack_swap_to:
              push_caller_save_regs
              mov %rsp, 8(%rdi)
              mov 0(%rdi), %rsp
              pop_caller_save_regs
              ret

              .data
              .global stack_min_size
              .global stack_alignment
stack_min_size: .int 8*8
stack_alignment: .int 16
