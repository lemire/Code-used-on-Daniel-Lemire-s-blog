/**
* Fast modulo reduction
*/

#include <stdio.h>
#include <stdint.h>
#include <x86intrin.h>

#define RDTSC_START(cycles)                                                   \
    do {                                                                      \
        register unsigned cyc_high, cyc_low;                                  \
        __asm volatile(                                                       \
            "cpuid\n\t"                                                       \
            "rdtsc\n\t"                                                       \
            "mov %%edx, %0\n\t"                                               \
            "mov %%eax, %1\n\t"                                               \
            : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx", "%rdx"); \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                      \
    } while (0)

#define RDTSC_FINAL(cycles)                                                   \
    do {                                                                      \
        register unsigned cyc_high, cyc_low;                                  \
        __asm volatile(                                                       \
            "rdtscp\n\t"                                                      \
            "mov %%edx, %0\n\t"                                               \
            "mov %%eax, %1\n\t"                                               \
            "cpuid\n\t"                                                       \
            : "=r"(cyc_high), "=r"(cyc_low)::"%rax", "%rbx", "%rcx", "%rdx"); \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                      \
    } while (0)



#define BEST_TIME(test, answer, repeat, size)                         \
    do {                                                              \
        printf("%s: ", #test);                                        \
        fflush(NULL);                                                 \
        uint64_t cycles_start, cycles_final, cycles_diff;             \
        uint64_t min_diff = (uint64_t)-1;                             \
        int wrong_answer = 0;                                         \
        for (int i = 0; i < repeat; i++) {                            \
            __asm volatile("" ::: /* pretend to clobber */ "memory"); \
            RDTSC_START(cycles_start);                                \
            if (test != answer) wrong_answer = 1;                     \
            RDTSC_FINAL(cycles_final);                                \
            cycles_diff = (cycles_final - cycles_start);              \
            if (cycles_diff < min_diff) min_diff = cycles_diff;       \
        }                                                             \
        uint64_t S = (uint64_t)size;                                  \
        float cycle_per_op = (min_diff) / (float)S;                   \
        printf(" %.2f cycles per operation", cycle_per_op);           \
        if (wrong_answer) printf(" [ERROR]");                         \
        printf("\n");                                                 \
        fflush(NULL);                                                 \
    } while (0)


uint32_t  mod23(uint32_t a) {
  return a % 23;
}

/**
(gdb) disas mod23
Dump of assembler code for function mod23:
   0x0000000100000cb0 <+0>:	push   %rbp
   0x0000000100000cb1 <+1>:	mov    %rsp,%rbp
   0x0000000100000cb4 <+4>:	mov    %edi,%eax
   0x0000000100000cb6 <+6>:	mov    $0xb21642c9,%ecx
   0x0000000100000cbb <+11>:	imul   %rax,%rcx
   0x0000000100000cbf <+15>:	shr    $0x24,%rcx
   0x0000000100000cc3 <+19>:	imul   $0x17,%ecx,%eax
   0x0000000100000cc6 <+22>:	sub    %eax,%edi
   0x0000000100000cc8 <+24>:	mov    %edi,%eax
   0x0000000100000cca <+26>:	pop    %rbp
   0x0000000100000ccb <+27>:	retq
   0x0000000100000ccc <+28>:	nopl   0x0(%rax)
*/

uint32_t altmod23(uint32_t a) {
    return ( ((2987803337*a) & ((1ULL<<36)-1)) * 23 ) >> 36;
}


uint32_t fastmod23(uint32_t a) {
    uint64_t lowbits =  UINT64_C(802032351030850071) * a; // high 64 bits of this mult is the division
    // we use the low bits to retrieve the modulo
    uint64_t highbits;
    _mulx_u64(lowbits,23,(long long unsigned int *) &highbits);
    return highbits;
}
/***
(gdb) disas fastmod23
Dump of assembler code for function fastmod23:
   0x0000000100000cf0 <+0>:	push   %rbp
   0x0000000100000cf1 <+1>:	mov    %rsp,%rbp
   0x0000000100000cf4 <+4>:	mov    %edi,%eax
   0x0000000100000cf6 <+6>:	movabs $0xb21642c8590b217,%rdx
   0x0000000100000d00 <+16>:	imul   %rax,%rdx
   0x0000000100000d04 <+20>:	mov    $0x17,%eax
   0x0000000100000d09 <+25>:	mulx   %rax,%rcx,%rax
   0x0000000100000d0e <+30>:	pop    %rbp
   0x0000000100000d0f <+31>:	retq
End of assembler dump.
****/


uint32_t sumofmod23(uint32_t maxval) {
  uint32_t sumofmods = 0;
  // we use a test that prevents the use of fancy compiler tricks like trivial vectorization
  for(uint32_t k = 0; k < maxval; ++k) sumofmods += mod23(k + sumofmods);
  return sumofmods;
}


uint32_t altsumofmod23(uint32_t maxval) {
  uint32_t sumofmods = 0;
  // we use a test that prevents the use of fancy compiler tricks like trivial vectorization
  for(uint32_t k = 0; k < maxval; ++k) sumofmods += altmod23(k + sumofmods);
  return sumofmods;
}

uint32_t fastsumofmod23(uint32_t maxval) {
  uint32_t sumofmods = 0;
  // we use a test that prevents the use of fancy compiler tricks like trivial vectorization
  for(uint32_t k = 0; k < maxval; ++k) sumofmods += fastmod23(k + sumofmods);
  return sumofmods;
}

int main() {
  uint32_t sumofmods = 0;
  const uint32_t maxval = 1000000;
  for(uint32_t k = 0; k < maxval; ++k) sumofmods += ( k + sumofmods )  % 23;
  const int repeat = 5;
  BEST_TIME(sumofmod23(maxval), sumofmods, repeat, maxval) ;
  BEST_TIME(altsumofmod23(maxval), sumofmods, repeat, maxval) ;
  BEST_TIME(fastsumofmod23(maxval), sumofmods, repeat, maxval) ;



  for(uint32_t x = 1; x !=0; x++) {
    if(mod23(x) != altmod23(x)) printf("%x\n",x);
    if(mod23(x) != fastmod23(x)) printf("%x\n",x);
  }
}
