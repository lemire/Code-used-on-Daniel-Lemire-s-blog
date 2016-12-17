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


uint64_t  mod23(uint64_t a) {
  return a % 23;
}

/**
(gdb) disas mod23
Dump of assembler code for function mod23:
   0x0000000100000a70 <+0>:	mov    %edi,%eax
   0x0000000100000a72 <+2>:	mov    $0xb21642c9,%edx
   0x0000000100000a77 <+7>:	mul    %edx
   0x0000000100000a79 <+9>:	mov    %edx,%eax
   0x0000000100000a7b <+11>:	shr    $0x4,%eax
   0x0000000100000a7e <+14>:	imul   $0x17,%eax,%eax
   0x0000000100000a81 <+17>:	sub    %eax,%edi
   0x0000000100000a83 <+19>:	mov    %edi,%eax
   0x0000000100000a85 <+21>:	retq
   0x0000000100000a86 <+22>:	nopw   %cs:0x0(%rax,%rax,1)
*/

/***
(gdb) disas fastmod23
Dump of assembler code for function fastmod23:
   0x0000000100000ab0 <+0>:	movabs $0xb21642c8590b217,%rdx
   0x0000000100000aba <+10>:	mov    %edi,%eax
   0x0000000100000abc <+12>:	imul   %rdx,%rax
   0x0000000100000ac0 <+16>:	mov    $0x17,%edx
   0x0000000100000ac5 <+21>:	mul    %rdx
   0x0000000100000ac8 <+24>:	mov    %rdx,%rax
   0x0000000100000acb <+27>:	retq
   0x0000000100000acc <+28>:	nopl   0x0(%rax)
End of assembler dump.
****/

//#define MUL64(rh,rl,i1,i2) asm ("mulq %3" : "=a"(rl), "=d"(rh) : "a"(i1), "r"(i2) : "cc")

#define MUL64HIGH(rh,i1,i2) asm ("mulq %2" : "=d"(rh) : "a"(i1), "r"(i2) : "cc")

uint32_t fastdiv23(uint64_t a) {
    uint64_t highbits;
    MUL64HIGH(highbits,(uint64_t)a,UINT64_C(802032351030850071));
    return highbits;
}


uint32_t fastmod23(uint64_t a) {
    uint64_t lowbits =  UINT64_C(802032351030850071) * a; // high 64 bits of this mult is the division
    // we use the low bits to retrieve the modulo
    uint64_t highbits;
    MUL64HIGH(highbits,lowbits,UINT64_C(23));
    return highbits;
}



uint64_t sumofmod23(uint64_t maxval) {
  uint64_t sumofmods = 0;
  // we use a test that prevents the use of fancy compiler tricks like trivial vectorization
  for(uint64_t k = 0; k < maxval; ++k) sumofmods += mod23(k + sumofmods);
  return sumofmods;
}


uint64_t fastsumofmod23(uint64_t maxval) {
  uint64_t sumofmods = 0;
  // we use a test that prevents the use of fancy compiler tricks like trivial vectorization
  for(uint64_t k = 0; k < maxval; ++k) sumofmods += fastmod23(k + sumofmods);
  return sumofmods;
}

int main() {
  uint64_t sumofmods = 0;
  const uint64_t maxval = 1000000;
  for(uint64_t k = 0; k < maxval; ++k) sumofmods += ( k + sumofmods )  % 23;
  const int repeat = 5;
  BEST_TIME(sumofmod23(maxval), sumofmods, repeat, maxval) ;
  BEST_TIME(fastsumofmod23(maxval), sumofmods, repeat, maxval) ;



  for(uint64_t x = 1; x !=0; x++) {
    if(mod23(x) != fastmod23(x)) printf("%x\n",x);
  }
}
