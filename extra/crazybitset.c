// gcc -fno-inline -march=native -std=c99 -Wall -O3 -g  bitset.c -o bitset
// clang also wants -mavx2 (whereas icc complains about it)
// -fno-inline is optional, but makes 'perf record -F10000 bitset'
// give clearer functions in 'perf report'.

#include <x86intrin.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define RDTSC_START(cycles)  do {                                       \
        uint32_t cyc_high, cyc_low;                                     \
        __asm volatile("cpuid\n"                                        \
                       "rdtsc\n"                                        \
                       "mov %%edx, %0\n"                                \
                       "mov %%eax, %1" :                                \
                       "=r" (cyc_high),                                 \
                       "=r"(cyc_low) :                                  \
                       : /* no read only */                             \
                       "%rax", "%rbx", "%rcx", "%rdx" /* clobbers */    \
                       );                                               \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                \
    } while (0)

#define RDTSC_STOP(cycles)  do {                                        \
        uint32_t cyc_high, cyc_low;                                     \
        __asm volatile("rdtscp\n"                                       \
                       "mov %%edx, %0\n"                                \
                       "mov %%eax, %1\n"                                \
                       "cpuid" :                                        \
                       "=r"(cyc_high),                                  \
                       "=r"(cyc_low) :                                  \
                       /* no read only registers */ :                   \
                       "%rax", "%rbx", "%rcx", "%rdx" /* clobbers */    \
                       );                                               \
        (cycles) = ((uint64_t)cyc_high << 32) | cyc_low;                \
    } while (0)

__attribute__ ((noinline))
uint64_t rdtsc_overhead_func(uint64_t dummy) {
    return dummy;
}

uint64_t global_rdtsc_overhead = (uint64_t) UINT64_MAX;

#define RDTSC_SET_OVERHEAD(test, repeat)                                \
    do {                                                                \
        uint64_t cycles_start, cycles_final, cycles_diff;               \
        uint64_t min_diff = UINT64_MAX;                                 \
        for (size_t i = 0; i < repeat; i++) {                           \
            __asm volatile("" ::: /* pretend to clobber */ "memory");   \
            RDTSC_START(cycles_start);                                  \
            test;                                                       \
            RDTSC_STOP(cycles_final);                                   \
            cycles_diff = (cycles_final - cycles_start);                \
            if (cycles_diff < min_diff) min_diff = cycles_diff;         \
        }                                                               \
        global_rdtsc_overhead = min_diff;                               \
        printf("rdtsc_overhead set to %ld\n", global_rdtsc_overhead);   \
    } while (0)                                                         \

#define RDTSC_LOOP(test, answer, repeat, num_vecs)                      \
    do {                                                                \
        if (global_rdtsc_overhead == UINT64_MAX) {                      \
            RDTSC_SET_OVERHEAD(rdtsc_overhead_func(1), repeat);         \
        }                                                               \
        printf("%36s: ", #test);                                        \
        fflush(NULL);							\
        uint64_t cycles_start, cycles_final, cycles_diff;               \
        int wrong_answer = 0;						\
        uint64_t result;                                                \
        uint64_t min_diff = UINT64_MAX;					\
        for (size_t i = 0; i < repeat; i++) {				\
            __asm volatile("" ::: /* pretend to clobber */ "memory");   \
            RDTSC_START(cycles_start);                                  \
            result = test;                                              \
            RDTSC_STOP(cycles_final);                                   \
            if (answer != result) wrong_answer = 1;                     \
            cycles_diff = (cycles_final - cycles_start);                \
            if (cycles_diff < min_diff) min_diff = cycles_diff;         \
        }                                                               \
        if (min_diff <= global_rdtsc_overhead) min_diff = 0;		\
        else min_diff = min_diff - global_rdtsc_overhead;               \
        float cycles_per_vec = min_diff / (double)(num_vecs);           \
        printf(" %.2f cycles per vector", cycles_per_vec);              \
        printf(" (%ld cycs / %ld vecs)", min_diff, (uint64_t) num_vecs); \
        if (wrong_answer) printf(" ERROR: expected %ld, got %ld",       \
                                 (uint64_t)answer, (uint64_t)result);   \
        else printf(" %8ld OK", (uint64_t)result);                      \
        printf("\n");							\
        fflush(NULL);							\
        } while (0)

// Likwid (https://github.com/RRZE-HPC/likwid) allows access to CPU performance counters.
// Likwid can be difficult to install, but allows measurement of distinct sections of code.
// I've symlinked 'likwid-perfctr' to 'likwid' on the machines I've installed it on.
// gcc -fno-inline -march=native -std=c99 -Wall -O3 -g bitset.c -DLIKWID -llikwid bitset_likwid
// Skylake:
// likwid -m -g MEM_INST_RETIRED_ALL_STORES:PMC0,UOPS_DISPATCHED_PORT_PORT_7:PMC1,UOPS_DISPATCHED_PORT_PORT_2:PMC2,UOPS_DISPATCHED_PORT_PORT_3:PMC3 -C 2 ./bitset_likwid
// likwid -m -g UOPS_DISPATCHED_PORT_PORT_0:PMC0,UOPS_DISPATCHED_PORT_PORT_1:PMC1,UOPS_DISPATCHED_PORT_PORT_5:PMC2,UOPS_DISPATCHED_PORT_PORT_6:PMC3 -C 2 ./bitset_likwid
// likwid -m -g IDQ_MITE_UOPS:PMC0,IDQ_DSB_UOPS:PMC1,LSD_UOPS:PMC2,IDQ_MS_UOPS:PMC3 -C 2 ./bitset
// likwid -m -g UOPS_EXECUTED_CYCLES_GE_1_UOPS_EXEC:PMC0,UOPS_EXECUTED_CYCLES_GE_2_UOPS_EXEC:PMC1,UOPS_EXECUTED_CYCLES_GE_3_UOPS_EXEC:PMC2,UOPS_EXECUTED_CYCLES_GE_4_UOPS_EXEC:PMC3 -C 2 ./bitset_likwid
// Haswell:
// likwid -m -g MEM_UOP_RETIRED_STORES:PMC0,UOPS_EXECUTED_PORT_PORT_7:PMC1,UOPS_EXECUTED_PORT_PORT_2:PMC2,UOPS_EXECUTED_PORT_PORT_3:PMC3 -C 2 ./bitset_likwid

#ifdef LIKWID
#include <likwid.h>
static const char *likwid_safe_name(const char *name, char *buffer) {
    strcpy(buffer, name);
    char *chop = strchr(buffer, '(');
    if (chop) *chop = 0;
    return buffer;
}
#define LIKWID_LOOP(test, answer, repeat, num_vecs) do {                \
        printf("%36s: ", #test);                                        \
        fflush(NULL);							\
        int wrong_answer = 0;						\
        uint64_t result;                                                \
        char safe_name[256];                                            \
        likwid_safe_name(#test, safe_name);                             \
        likwid_markerStartRegion(safe_name);                            \
        for (size_t i = 0; i < repeat; i++) {				\
            __asm volatile("" ::: /* pretend to clobber */ "memory");   \
            result = test;                                              \
            if (result != answer) wrong_answer = 1;                     \
        }                                                               \
        likwid_markerStopRegion(safe_name);                             \
        if (wrong_answer) printf(" ERROR: expected %ld, got %ld",       \
                                 (uint64_t)answer, (uint64_t)result);   \
        else printf(" %8ld OK", (uint64_t)result);                      \
        printf("\n");							\
        fflush(NULL);							\
    } while (0)
#define TIMING_LOOP LIKWID_LOOP
#else
#define TIMING_LOOP RDTSC_LOOP
#endif

#ifdef IACA
// Compile with "-DIACA -o iaca.o" to use Intel's port analyzer
// iaca  -mark 0 -64 -arch HSW -analysis THROUGHPUT iaca.o
// Note that the exact start of the measured section is often off
// by a few instructions.  This can be adjusted by editing the
// assembly and putting the marker in the desired location.
// Also note that as of Jan 2016, IACA
// only has emulation through Haswell.  One important Skylake
// difference for this code is that SBB is down from 2 to 1 cycle
// and has only one microop on P0|6.  Also note that IACA is not
// aware of all factors that will affect actual performance.
#include </opt/intel/iaca/include/iacaMarks.h>
#else
#define IACA_START
#define IACA_END
#endif

#define BITSET_BITS (1 << 16)
#define BITSET_BYTES (BITSET_BITS / 8)

typedef __m256i ymm_t;
typedef __m128i xmm_t;
#define XMM_INT(vec) (*(xmm_t *)(&vec))
#define XMM_FLOAT(vec) (*(__m128 *)(&vec))

#define ASM_STORE_VEC_OFFSET_BASE(vec, offset, base)    \
    __asm volatile ("vmovdqu %0, %c1(%2)\n" :           \
                    /* no registers written */  :       \
                    /* read vec */   "x" (vec),         \
                    /* read const */ "i" (offset),      \
                    /* read reg */   "r" (base) :       \
                    /* clobbers */ "memory")

#define ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(vec, offset, base, index, scale) \
    __asm volatile ("vmovdqu %c1(%2,%3,%c4), %0\n" :                    \
                    /* write vec */  "=x" (vec) :                       \
                    /* read const */ "i" (offset),                      \
                    /* read reg */   "r" (base),                        \
                    /* read reg */   "r" (index),                       \
                    /* read const*/  "i" (scale))

#define ASM_ADD_CONST(reg, const)                       \
    __asm volatile ("add %1, %0\n" :                    \
                    /* read/write reg */ "+r" (reg)  :  \
                    /* read const */     "i" (const))

#define ASM_OR_VEC(dst, src)                            \
    __asm volatile ("vpor %1, %0, %0\n" :               \
                    /* read/write vec */ "+x" (dst)  :  \
                    /* read vec */       "x"  (src))

// set bit at position testBit within testByte to 1 and
// copy cmovDst to cmovSrc if that bit was previously clear
#define ASM_SET_BIT_INC_WAS_CLEAR(testByte, testBit, count)     \
    __asm volatile ("bts %2, %0\n"                              \
                    "sbb $-1, %1\n" :                           \
                    "+r" (testByte), /* read/write */           \
                    "+r" (count) :   /* read/write */           \
                    "r" (testBit)    /* read only */            \
                    )

#define ASM_CLEAR_BIT_DEC_WAS_SET(testByte, testBit, count)     \
    __asm volatile ("btr %2, %0\n"                              \
                    "sbb $0, %1\n" :                            \
                    "+r" (testByte), /* read/write */           \
                    "+r" (count) :   /* read/write */           \
                    "r" (testBit)    /* read only */            \
                    )

#define ASM_SHIFT_RIGHT(srcReg, bitsReg, destReg)       \
    __asm volatile ("shrx %1, %2, %0" :                 \
                    "=r" (destReg): /* read/write */    \
                    "r" (bitsReg),  /* read only */     \
                    "r" (srcReg)    /* read only */     \
                    )


uint64_t bitset_set_bit(uint64_t *array, uint64_t pos,
                        uint64_t card) {
    uint64_t shift = 6;
    uint64_t offset;
    ASM_SHIFT_RIGHT(pos, shift, offset);
    uint64_t load = array[offset];
    ASM_SET_BIT_INC_WAS_CLEAR(load, pos, card);
    array[offset] = load;
    return card;
}

uint64_t bitset_set_list_intrinsic(void *bitset, uint64_t card,
                         uint16_t *list, uint64_t length) {
    uint64_t offset, load, pos;
    uint16_t *end = list + length;
    while(list != end) {
      pos =  *(uint16_t *)  list;
      offset = pos >> 6;
      load = (uint64_t *) bitset[offset];
      card += _bittestandset64(&load, pos);
      list ++;
    }
}

uint64_t bitset_set_list(void *bitset, uint64_t card,
                         uint16_t *list, uint64_t length) {
    uint64_t offset, load, pos;
    uint64_t shift = 3;
    uint16_t *end = list + length;

    IACA_START;
#if 1
    __asm volatile("1:\n"
                   "movzwq (%[list]), %[pos]\n"
                   "shrx %[shift], %[pos], %[offset]\n"
                   "movzbq (%[bitset],%[offset],1), %[load]\n"
                   "bts %[pos], %[load]\n"
                   "mov %b[load], (%[bitset],%[offset],1)\n"
                   "sbb $-1, %[card]\n"
                   "add $2, %[list]\n"
                   "cmp %[list], %[end]\n"
                   "jnz 1b" :
                   [card] "+&r" (card),
                   [list] "+&r" (list),
                   [load] "=&r" (load),
                   [pos] "=&r" (pos),
                   [offset] "=&r" (offset) :
                   [end] "r" (end),
                   [bitset] "r" (bitset),
                   [shift] "r" (shift));
#else
    /* seems like this should be slightly faster, but it's not */
    int64_t neg = -length;
    __asm volatile("1:\n"
                   "movzwq (%[end],%[neg],2), %[pos]\n"
                   "shrx %[shift], %[pos], %[offset]\n"
                   "movzbq (%[bitset],%[offset],1), %[load]\n"
                   "bts %[pos], %[load]\n"
                   "mov %b[load], (%[bitset],%[offset],1)\n"
                   "sbb $-1, %[card]\n"
                   "inc %[neg]\n"
                   "jnz 1b" :
                   [card] "+&r" (card),
                   [neg] "+&r" (neg),
                   [load] "=&r" (load),
                   [pos] "=&r" (pos),
                   [offset] "=&r" (offset) :
                   [end] "r" (end),
                   [bitset] "r" (bitset),
                   [shift] "r" (shift));
#endif
    IACA_END;
    return card;
}

// presumption here is that moving 8-bit pieces  will have less
// chance of causing 'interference' between writes and future reads
uint64_t bitset_clear_list(void *bitset, uint64_t card,
                           uint16_t *list, uint64_t length) {
    uint64_t offset, load, pos;
    uint64_t shift = 3;
    uint16_t *end = list + length;


    IACA_START;
    __asm volatile("1:\n"
                   "movzwq (%[list]), %[pos]\n"
                   "shrx %[shift], %[pos], %[offset]\n"
                   "movzbq (%[bitset],%[offset],1), %[load]\n"
                   "btr %[pos], %[load]\n"
                   "mov %b[load], (%[bitset],%[offset],1)\n"
                   "sbb $0, %[card]\n"
                   "add $2, %[list]\n"
                   "cmp %[list], %[end]\n"
                   "jnz 1b" :
                   [card] "+&r" (card),
                   [list] "+&r" (list),
                   [load] "=&r" (load),
                   [pos] "=&r" (pos),
                   [offset] "=&r" (offset) :
                   [end] "r" (end),
                   [bitset] "r" (bitset),
                   [shift] "r" (shift));
    IACA_END;
    return card;
}

// actual function will presumably set bitset->card directly
uint64_t bitset_clear_bit(uint64_t *array, uint64_t pos,
                          uint64_t card) {
    uint64_t shift = 6;
    uint64_t offset;
    ASM_SHIFT_RIGHT(pos, shift, offset);
    uint64_t load = array[offset];
    ASM_CLEAR_BIT_DEC_WAS_SET(load, pos, card);
    array[offset] = load;
    return card;
}

uint64_t bitset_or_nocard(void *in1, void *in2, void *out) {
    char *end1 = (char *)in1 + BITSET_BYTES;
    char *end2 = (char *)in2 + BITSET_BYTES;
    int64_t neg = -BITSET_BYTES;

    IACA_START;
    do {
        __m256i A1, A2;
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A1, 0, end1, neg, 1);
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A2, 0, end2, neg, 1);
        ASM_OR_VEC(A1, A2);
        ASM_STORE_VEC_OFFSET_BASE(A1, 0, out);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A1, 32, end1, neg, 1);
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A2, 32, end2, neg, 1);
        ASM_OR_VEC(A1, A2);
        ASM_STORE_VEC_OFFSET_BASE(A1, 32, out);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A1, 64, end1, neg, 1);
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A2, 64, end2, neg, 1);
        ASM_OR_VEC(A1, A2);
        ASM_STORE_VEC_OFFSET_BASE(A1, 64, out);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A1, 96, end1, neg, 1);
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A2, 96, end2, neg, 1);
        ASM_OR_VEC(A1, A2);
        ASM_STORE_VEC_OFFSET_BASE(A1, 96, out);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A1, 128, end1, neg, 1);
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A2, 128, end2, neg, 1);
        ASM_OR_VEC(A1, A2);
        ASM_STORE_VEC_OFFSET_BASE(A1, 128, out);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A1, 160, end1, neg, 1);
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A2, 160, end2, neg, 1);
        ASM_OR_VEC(A1, A2);
        ASM_STORE_VEC_OFFSET_BASE(A1, 160, out);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A1, 192, end1, neg, 1);
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A2, 192, end2, neg, 1);
        ASM_OR_VEC(A1, A2);
        ASM_STORE_VEC_OFFSET_BASE(A1, 192, out);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A1, 224, end1, neg, 1);
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A2, 224, end2, neg, 1);
        ASM_OR_VEC(A1, A2);
        ASM_STORE_VEC_OFFSET_BASE(A1, 224, out);

        ASM_ADD_CONST(out, 256);
    } while (neg += 256);

    IACA_END;
    return -1;
}

uint64_t bitset_or_card(void *in1, void *in2, void *out) {
    char *end1 = (char *)in1 + BITSET_BYTES;
    char *end2 = (char *)in2 + BITSET_BYTES;
    const ymm_t mask = _mm256_set1_epi8(0x0f);
    const ymm_t zero = _mm256_setzero_si256();
    const ymm_t shuf =
        _mm256_setr_epi8(0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
                         0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4);
    ymm_t total = _mm256_setzero_si256();
    int64_t neg = -BITSET_BYTES;

    IACA_START;
    do {
        ymm_t A1, A2, B1, B2;
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A1, 0, end1, neg, 1);
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A2, 0, end2, neg, 1);
        B1 = _mm256_or_si256(A2, A1);
        ASM_STORE_VEC_OFFSET_BASE(B1, 0, out);
        B2 = _mm256_srli_epi32(B1, 4);
        B1 = _mm256_and_si256(B1, mask);
        B2 = _mm256_and_si256(B2, mask);
        ymm_t subtotal = _mm256_shuffle_epi8(shuf, B1);
        B2 = _mm256_shuffle_epi8(shuf, B2);
        subtotal = _mm256_add_epi8(subtotal,B2);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A1, 32, end1, neg, 1);
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A2, 32, end2, neg, 1);
        B1 = _mm256_or_si256(A2, A1);
        ASM_STORE_VEC_OFFSET_BASE(B1, 32, out);
        B2 = _mm256_srli_epi32(B1, 4);
        B1 = _mm256_and_si256(B1, mask);
        B2 = _mm256_and_si256(B2, mask);
        B1 = _mm256_shuffle_epi8(shuf, B1);
        B2 = _mm256_shuffle_epi8(shuf, B2);
        subtotal = _mm256_add_epi8(subtotal, B1);
        subtotal = _mm256_add_epi8(subtotal, B2);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A1, 64, end1, neg, 1);
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A2, 64, end2, neg, 1);
        B1 = _mm256_or_si256(A2, A1);
        ASM_STORE_VEC_OFFSET_BASE(B1, 64, out);
        B2 = _mm256_srli_epi32(B1, 4);
        B1 = _mm256_and_si256(B1, mask);
        B2 = _mm256_and_si256(B2, mask);
        B1 = _mm256_shuffle_epi8(shuf, B1);
        B2 = _mm256_shuffle_epi8(shuf, B2);
        subtotal = _mm256_add_epi8(subtotal, B1);
        subtotal = _mm256_add_epi8(subtotal, B2);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A1, 96, end1, neg, 1);
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A2, 96, end2, neg, 1);
        B1 = _mm256_or_si256(A2, A1);
        ASM_STORE_VEC_OFFSET_BASE(B1, 96, out);
        B2 = _mm256_srli_epi32(B1, 4);
        B1 = _mm256_and_si256(B1, mask);
        B2 = _mm256_and_si256(B2, mask);
        B1 = _mm256_shuffle_epi8(shuf, B1);
        B2 = _mm256_shuffle_epi8(shuf, B2);
        subtotal = _mm256_add_epi8(subtotal, B1);
        subtotal = _mm256_add_epi8(subtotal, B2);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A1, 128, end1, neg, 1);
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A2, 128, end2, neg, 1);
        B1 = _mm256_or_si256(A2, A1);
        ASM_STORE_VEC_OFFSET_BASE(B1, 128, out);
        B2 = _mm256_srli_epi32(B1, 4);
        B1 = _mm256_and_si256(B1, mask);
        B2 = _mm256_and_si256(B2, mask);
        B1 = _mm256_shuffle_epi8(shuf, B1);
        B2 = _mm256_shuffle_epi8(shuf, B2);
        subtotal = _mm256_add_epi8(subtotal, B1);
        subtotal = _mm256_add_epi8(subtotal, B2);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A1, 160, end1, neg, 1);
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A2, 160, end2, neg, 1);
        B1 = _mm256_or_si256(A2, A1);
        ASM_STORE_VEC_OFFSET_BASE(B1, 160, out);
        B2 = _mm256_srli_epi32(B1, 4);
        B1 = _mm256_and_si256(B1,mask);
        B2 = _mm256_and_si256(B2, mask);
        B1 = _mm256_shuffle_epi8(shuf, B1);
        B2 = _mm256_shuffle_epi8(shuf, B2);
        subtotal = _mm256_add_epi8(subtotal, B1);
        subtotal = _mm256_add_epi8(subtotal, B2);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A1, 192, end1, neg, 1);
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A2, 192, end2, neg, 1);
        B1 = _mm256_or_si256(A2, A1);
        ASM_STORE_VEC_OFFSET_BASE(B1, 192, out);
        B2 = _mm256_srli_epi32(B1, 4);
        B1 = _mm256_and_si256(B1, mask);
        B2 = _mm256_and_si256(B2, mask);
        B1 = _mm256_shuffle_epi8(shuf, B1);
        B2 = _mm256_shuffle_epi8(shuf, B2);
        subtotal = _mm256_add_epi8(subtotal, B1);
        subtotal = _mm256_add_epi8(subtotal, B2);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A1, 224, end1, neg, 1);
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A2, 224, end2, neg, 1);
        B1 = _mm256_or_si256(A2, A1);
        ASM_STORE_VEC_OFFSET_BASE(B1, 224, out);
        B2 = _mm256_srli_epi32(B1, 4);
        B1 = _mm256_and_si256(B1, mask);
        B2 = _mm256_and_si256(B2, mask);
        B1 = _mm256_shuffle_epi8(shuf, B1);
        B2 = _mm256_shuffle_epi8(shuf, B2);
        subtotal = _mm256_add_epi8(subtotal, B1);
        subtotal = _mm256_add_epi8(subtotal, B2);

        subtotal = _mm256_sad_epu8(zero, subtotal);
        total = _mm256_add_epi64(total, subtotal);
        ASM_ADD_CONST(out, 256);
    } while (neg += 256);

    xmm_t tmp1 = _mm256_extracti128_si256(total, 1);
    xmm_t sum = _mm_add_epi64(tmp1, XMM_INT(total));
    __m128 tmp2 = _mm_movehl_ps(XMM_FLOAT(zero), XMM_FLOAT(sum));
    sum = _mm_add_epi64(sum, XMM_INT(tmp2));
    uint64_t card = _mm_cvtsi128_si32(sum);

    IACA_END;
    return card;
}

uint64_t bitset_card_only(void *in) {
    char *end = (char *)in + BITSET_BYTES;
    const ymm_t mask = _mm256_set1_epi8(0x0f);
    const ymm_t zero = _mm256_setzero_si256();
    const ymm_t shuf =
        _mm256_setr_epi8(0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
                         0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4);
    ymm_t total = _mm256_setzero_si256();
    int64_t neg = -BITSET_BYTES;

    IACA_START;
    do {
#if 0   // no improvement over simpler version below
        ymm_t A1, A2, B1, B2;
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A1, 0, end, neg, 1);
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(B1, 32, end, neg, 1);
        A2 = _mm256_srli_epi32(A1, 4);
        B2 = _mm256_srli_epi32(B1, 4);
        A1 = _mm256_and_si256(A1, mask);
        A2 = _mm256_and_si256(A2, mask);
        ymm_t subtotal = _mm256_shuffle_epi8(shuf, A1);
        A2 = _mm256_shuffle_epi8(shuf, A2);
        B1 = _mm256_and_si256(B1, mask);
        B2 = _mm256_and_si256(B2, mask);
        B1 = _mm256_shuffle_epi8(shuf, B1);
        B2 = _mm256_shuffle_epi8(shuf, B2);
        subtotal = _mm256_add_epi8(subtotal,A2);
        subtotal = _mm256_add_epi8(subtotal, B1);
        subtotal = _mm256_add_epi8(subtotal, B2);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A1, 64, end, neg, 1);
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(B1, 96, end, neg, 1);
        A2 = _mm256_srli_epi32(A1, 4);
        B2 = _mm256_srli_epi32(B1, 4);
        A1 = _mm256_and_si256(A1, mask);
        A2 = _mm256_and_si256(A2, mask);
        A1 = _mm256_shuffle_epi8(shuf, A1);
        A2 = _mm256_shuffle_epi8(shuf, A2);
        B1 = _mm256_and_si256(B1, mask);
        B2 = _mm256_and_si256(B2, mask);
        B1 = _mm256_shuffle_epi8(shuf, B1);
        B2 = _mm256_shuffle_epi8(shuf, B2);
        subtotal = _mm256_add_epi8(subtotal, A1);
        subtotal = _mm256_add_epi8(subtotal, A2);
        subtotal = _mm256_add_epi8(subtotal, B1);
        subtotal = _mm256_add_epi8(subtotal, B2);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A1, 128, end, neg, 1);
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(B1, 160, end, neg, 1);
        A2 = _mm256_srli_epi32(A1, 4);
        B2 = _mm256_srli_epi32(B1, 4);
        A1 = _mm256_and_si256(A1, mask);
        A2 = _mm256_and_si256(A2, mask);
        A1 = _mm256_shuffle_epi8(shuf, A1);
        A2 = _mm256_shuffle_epi8(shuf, A2);
        B1 = _mm256_and_si256(B1, mask);
        B2 = _mm256_and_si256(B2, mask);
        B1 = _mm256_shuffle_epi8(shuf, B1);
        B2 = _mm256_shuffle_epi8(shuf, B2);
        subtotal = _mm256_add_epi8(subtotal, A1);
        subtotal = _mm256_add_epi8(subtotal, A2);
        subtotal = _mm256_add_epi8(subtotal, B1);
        subtotal = _mm256_add_epi8(subtotal, B2);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(A1, 192, end, neg, 1);
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(B1, 224, end, neg, 1);
        A2 = _mm256_srli_epi32(A1, 4);
        B2 = _mm256_srli_epi32(B1, 4);
        A1 = _mm256_and_si256(A1, mask);
        A2 = _mm256_and_si256(A2, mask);
        A1 = _mm256_shuffle_epi8(shuf, A1);
        A2 = _mm256_shuffle_epi8(shuf, A2);
        B1 = _mm256_and_si256(B1, mask);
        B2 = _mm256_and_si256(B2, mask);
        B1 = _mm256_shuffle_epi8(shuf, B1);
        B2 = _mm256_shuffle_epi8(shuf, B2);
        subtotal = _mm256_add_epi8(subtotal, A1);
        subtotal = _mm256_add_epi8(subtotal, A2);
        subtotal = _mm256_add_epi8(subtotal, B1);
        subtotal = _mm256_add_epi8(subtotal, B2);
#else
        ymm_t B1, B2;
        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(B1, 0, end, neg, 1);
        B2 = _mm256_srli_epi32(B1, 4);
        B1 = _mm256_and_si256(B1,mask);
        B2 = _mm256_and_si256(B2, mask);
        ymm_t subtotal = _mm256_shuffle_epi8(shuf, B1);
        B2 = _mm256_shuffle_epi8(shuf, B2);
        subtotal = _mm256_add_epi8(subtotal,B2);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(B1, 32, end, neg, 1);
        B2 = _mm256_srli_epi32(B1, 4);
        B1 = _mm256_and_si256(B1, mask);
        B2 = _mm256_and_si256(B2, mask);
        B1 = _mm256_shuffle_epi8(shuf, B1);
        B2 = _mm256_shuffle_epi8(shuf, B2);
        subtotal = _mm256_add_epi8(subtotal, B1);
        subtotal = _mm256_add_epi8(subtotal, B2);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(B1, 64, end, neg, 1);
        B2 = _mm256_srli_epi32(B1, 4);
        B1 = _mm256_and_si256(B1, mask);
        B2 = _mm256_and_si256(B2, mask);
        B1 = _mm256_shuffle_epi8(shuf, B1);
        B2 = _mm256_shuffle_epi8(shuf, B2);
        subtotal = _mm256_add_epi8(subtotal, B1);
        subtotal = _mm256_add_epi8(subtotal, B2);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(B1, 96, end, neg, 1);
        B2 = _mm256_srli_epi32(B1, 4);
        B1 = _mm256_and_si256(B1, mask);
        B2 = _mm256_and_si256(B2, mask);
        B1 = _mm256_shuffle_epi8(shuf, B1);
        B2 = _mm256_shuffle_epi8(shuf, B2);
        subtotal = _mm256_add_epi8(subtotal, B1);
        subtotal = _mm256_add_epi8(subtotal, B2);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(B1, 128, end, neg, 1);
        B2 = _mm256_srli_epi32(B1, 4);
        B1 = _mm256_and_si256(B1, mask);
        B2 = _mm256_and_si256(B2, mask);
        B1 = _mm256_shuffle_epi8(shuf, B1);
        B2 = _mm256_shuffle_epi8(shuf, B2);
        subtotal = _mm256_add_epi8(subtotal, B1);
        subtotal = _mm256_add_epi8(subtotal, B2);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(B1, 160, end, neg, 1);
        B2 = _mm256_srli_epi32(B1, 4);
        B1 = _mm256_and_si256(B1, mask);
        B2 = _mm256_and_si256(B2, mask);
        B1 = _mm256_shuffle_epi8(shuf, B1);
        B2 = _mm256_shuffle_epi8(shuf, B2);
        subtotal = _mm256_add_epi8(subtotal, B1);
        subtotal = _mm256_add_epi8(subtotal, B2);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(B1, 192, end, neg, 1);
        B2 = _mm256_srli_epi32(B1, 4);
        B1 = _mm256_and_si256(B1, mask);
        B2 = _mm256_and_si256(B2, mask);
        B1 = _mm256_shuffle_epi8(shuf, B1);
        B2 = _mm256_shuffle_epi8(shuf, B2);
        subtotal = _mm256_add_epi8(subtotal, B1);
        subtotal = _mm256_add_epi8(subtotal, B2);

        ASM_LOAD_VEC_OFFSET_BASE_INDEX_SCALE(B1, 224, end, neg, 1);
        B2 = _mm256_srli_epi32(B1, 4);
        B1 = _mm256_and_si256(B1, mask);
        B2 = _mm256_and_si256(B2, mask);
        B1 = _mm256_shuffle_epi8(shuf, B1);
        B2 = _mm256_shuffle_epi8(shuf, B2);
        subtotal = _mm256_add_epi8(subtotal, B1);
        subtotal = _mm256_add_epi8(subtotal, B2);
#endif

        subtotal = _mm256_sad_epu8(zero, subtotal);
        total = _mm256_add_epi64(total, subtotal);
    } while (neg += 256);

    xmm_t tmp1 = _mm256_extracti128_si256(total, 1);
    xmm_t sum = _mm_add_epi64(tmp1, XMM_INT(total));
    __m128 tmp2 = _mm_movehl_ps(XMM_FLOAT(zero), XMM_FLOAT(sum));
    sum = _mm_add_epi64(sum, XMM_INT(tmp2));
    uint64_t card = _mm_cvtsi128_si32(sum);

    IACA_END;
    return card;
}

void *aligned_malloc(size_t alignment, size_t size) {
    void *aligned;
    if (posix_memalign(&aligned, alignment, size)) exit(1);
    return aligned;
}

uint64_t check_bytes(uint8_t *bytes, uint8_t expected, uint64_t length) {
    for (int i = 0; i < BITSET_BYTES; i++) {
        if (bytes[i] != expected) {
            printf("Error at bytes[%d] (expected %x, got %x)\n", i, expected, bytes[i]);
            return 1;
        }
    }
    return 0;
}

#define REPEAT 100000

int main(/* int argc, char **argv */) {

#ifdef LIKWID
    likwid_markerInit();
    likwid_markerThreadInit();
#endif

    uint16_t list[256];
    for (int i = 0; i < 256; i++) {
        list[i] = i * 256;
    }

    uint8_t *out = aligned_malloc(4096, BITSET_BYTES);
    memset(out, 0x00, BITSET_BYTES);

    uint64_t card;
    memset(out, 0x00, BITSET_BYTES);
    card = bitset_set_list(out, 0, list, 256);
    if (card != 256) printf("Error: expected %ld, got %ld\n", 256L, card);
    TIMING_LOOP(bitset_set_list(out, 256, list, 256), 256, REPEAT, 256);
    card = bitset_clear_list(out, 256, list, 256);
    if (card != 0) printf("Error: expected %ld, got %ld\n", 0L, card);
    check_bytes(out, 0x00, BITSET_BYTES);
    TIMING_LOOP(bitset_clear_list(out, 0, list, 256), 0, REPEAT, 256);

    card = 6 * BITSET_BYTES;
    memset(out, 0x77, BITSET_BYTES);
    TIMING_LOOP(bitset_card_only(out), card, REPEAT, 256);

    uint8_t *in1 = aligned_malloc(4096, BITSET_BYTES);
    uint8_t *in2 = aligned_malloc(4096, BITSET_BYTES);
    memset(in1, 0x71, BITSET_BYTES);
    memset(in2, 0x17, BITSET_BYTES);

    memset(out, 0x00, BITSET_BYTES);
    TIMING_LOOP(bitset_or_nocard(in1, in2, out), -1, REPEAT, 256);
    check_bytes(out, 0x77, BITSET_BYTES);

    memset(out, 0x00, BITSET_BYTES);
    TIMING_LOOP(bitset_or_card(in1, in2, out), card, REPEAT, 256);
    check_bytes(out, 0x77, BITSET_BYTES);

#ifdef LIKWID
    likwid_markerClose();
#endif

    return 0;
}
