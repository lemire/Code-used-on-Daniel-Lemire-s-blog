// gcc -march=native -O3 -o simdunion simdunion.c
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>


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


#include <x86intrin.h>
static bool is_sorted(__m128i x) {
    return ( (uint32_t)_mm_extract_epi32(x,0)<=(uint32_t)_mm_extract_epi32(x,1))
           && ( (uint32_t)_mm_extract_epi32(x,1)<=(uint32_t)_mm_extract_epi32(x,2))
           && ( (uint32_t)_mm_extract_epi32(x,2)<=(uint32_t)_mm_extract_epi32(x,3));
}

static void array_print(const uint32_t * x, uint32_t len) {
    for(uint32_t i = 0; i < len; i++) {
        printf("%u,",x[i]);
    }
    printf("\n");
}

static bool array_is_sorted(const uint32_t * x, uint32_t len) {
    for(uint32_t i = 1; i < len; i++) {
        if(x[i] < x[i-1]) return false;
    }
    return true;
}
static bool array_is_strictly_sorted(const uint32_t * x, uint32_t len) {
    for(uint32_t i = 1; i < len; i++) {
        if(x[i] <= x[i-1]) return false;
    }
    return true;
}


static void print32(__m128i x) {
    printf("%u %u %u %u \n", _mm_extract_epi32(x,0),_mm_extract_epi32(x,1),_mm_extract_epi32(x,2),_mm_extract_epi32(x,3));
}

// Inoue and Taura, SIMD- and Cache-Friendly Algorithm for Sorting an Array of Structures
static void sse_merge(__m128i *vA, __m128i *vB, // input 1 & 2
                      __m128i *vMin, __m128i *vMax) { // output
    __m128i vTmp;

    vTmp = _mm_min_epu32(*vA, *vB);
// vTmp = 0 1 4 5
    *vMax = _mm_max_epu32(*vA, *vB);
// vMax = 2 3 6 7
    vTmp = _mm_alignr_epi8(vTmp, vTmp, 4);
// vTmp = 1 4 5 0 (rotate the min values)
    *vMin = _mm_min_epu32(vTmp, *vMax);
// vMin = 1 3 5 0
    *vMax = _mm_max_epu32(vTmp, *vMax);
// vMax = 2 4 6 7
    vTmp = _mm_alignr_epi8(*vMin, *vMin, 4);
// vTmp = 3 5 0 1 (rotate min)
    *vMin = _mm_min_epu32(vTmp, *vMax);
// vMin = 2 4 0 1
    *vMax = _mm_max_epu32(vTmp, *vMax);
// vMax = 3 5 6 7
    vTmp = _mm_alignr_epi8(*vMin, *vMin, 4);
// rottate min vTmp = 4 0 1 2
    *vMin = _mm_min_epu32(vTmp, *vMax);
// vMin = 3 0 1 2
    *vMax = _mm_max_epu32(vTmp, *vMax);
// vMax = 4 5 6 7
    *vMin = _mm_alignr_epi8(*vMin, *vMin, 4);

}

static uint8_t uniqshuf[]= {0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf,
                            0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf,0xFF,0xFF,0xFF,0xFF,
                            0x0,0x1,0x2,0x3,0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf,0xFF,0xFF,0xFF,0xFF,
                            0x8,0x9,0xa,0xb,0xc,0xd,0xe,0xf,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                            0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0xc,0xd,0xe,0xf,0xFF,0xFF,0xFF,0xFF,
                            0x4,0x5,0x6,0x7,0xc,0xd,0xe,0xf,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                            0x0,0x1,0x2,0x3,0xc,0xd,0xe,0xf,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                            0xc,0xd,0xe,0xf,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                            0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xFF,0xFF,0xFF,0xFF,
                            0x4,0x5,0x6,0x7,0x8,0x9,0xa,0xb,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                            0x0,0x1,0x2,0x3,0x8,0x9,0xa,0xb,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                            0x8,0x9,0xa,0xb,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                            0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                            0x4,0x5,0x6,0x7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                            0x0,0x1,0x2,0x3,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                            0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
                           };


static int store_unique(__m128i old,__m128i new, uint32_t *  output) {

    __m128i vTmp = _mm_alignr_epi8(new, old, 16-4);
    int M = _mm_movemask_epi8(_mm_cmpeq_epi32(vTmp,new));//_pdep_u32(,0x1111);
    M=_pext_u32(M,0x1111);
    int numberofnewvalues = 4 - _mm_popcnt_u32(M);
    __m128i key =  _mm_lddqu_si128((const __m128i* )uniqshuf + M);
    __m128i val =  _mm_shuffle_epi8(new,key);
    _mm_storeu_si128((__m128i* )output,val);
    return numberofnewvalues;
}

static uint32_t unique(uint32_t *  out, uint32_t len) {
    uint32_t pos = 1;
    for(uint32_t i = 1; i < len; ++i) {
        if(out[i] != out[i-1]) {
            out[pos++] = out[i];
        }
    }
    return pos;
}


static uint32_t sse_unite_simple(uint32_t * __restrict__ array1, uint32_t length1 ,
                                 uint32_t *  __restrict__ array2, uint32_t length2, uint32_t *  __restrict__  output) {
    __m128i vA, vB, V, vMin,vMax;
    uint32_t * initoutput = output;
    assert(length1 % 4 == 0); // because I am lazy
    assert(length2 % 4 == 0); // because I am lazy
    assert(length1 != 0); // because I am lazy
    assert(length2 != 0); // because I am lazy
    uint32_t len1 = length1 / 4;
    uint32_t len2 = length2 / 4;
    uint32_t pos1 = 0;
    uint32_t pos2 = 0;
    // we start the machine
    vA = _mm_lddqu_si128((const __m128i* )array1 + pos1);
    pos1++;
    vB = _mm_lddqu_si128((const __m128i* )array2 + pos2);
    pos2++;
    uint32_t curA, curB;
    curA = (pos1 < len1) ? array1[4*pos1] : 0;
    curB = (pos2 < len2) ? array2[4*pos2] : 0;
    sse_merge(&vA,&vB,&vMin,&vMax);
    _mm_storeu_si128((__m128i* )output,vMin);
    output+= 4;
    while((pos1 < len1) && (pos2<len2)) {
        // code is not optimal, too much branching?
        if(curA <= curB) {
            V = _mm_lddqu_si128((const __m128i* )array1 + pos1);
            pos1++;
            curA = (pos1 < len1) ? array1[4*pos1] : 0;
        } else {
            V = _mm_lddqu_si128((const __m128i* )array2 + pos2);
            pos2++;
            curB = (pos2 < len2) ? array2[4*pos2] : 0;
        }
        sse_merge(&V,&vMax,&vMin,&vMax);
        _mm_storeu_si128((__m128i* )output,vMin);
        output+= 4;
    }
    //
    while(pos1<len1) {
        V = _mm_lddqu_si128((const __m128i* )array1 + pos1);
        pos1++;
        sse_merge(&V,&vMax,&vMin,&vMax);
        _mm_storeu_si128((__m128i* )output,vMin);
        output+= 4;
    }
    while(pos2<len2) {
        V = _mm_lddqu_si128((const __m128i* )array2 + pos2);
        pos2++;
        sse_merge(&V,&vMax,&vMin,&vMax);
        _mm_storeu_si128((__m128i* )output,vMin);
        output+=4;
    }
    _mm_storeu_si128((__m128i* )output,vMax);
    output+= 4;
    uint32_t len = output-initoutput  /*outpos * 4*/;
    return unique(initoutput, len);
}

static uint32_t sse_unite(uint32_t * __restrict__ array1, uint32_t length1 ,
                          uint32_t *  __restrict__ array2, uint32_t length2, uint32_t *  __restrict__  output) {
    __m128i vA, vB, V, vMin,vMax;
    __m128i laststore;
    uint32_t * initoutput = output;
    assert(length1 % 4 == 0); // because I am lazy
    assert(length2 % 4 == 0); // because I am lazy
    assert(length1 != 0); // because I am lazy
    assert(length2 != 0); // because I am lazy
    uint32_t len1 = length1 / 4;
    uint32_t len2 = length2 / 4;
    uint32_t pos1 = 0;
    uint32_t pos2 = 0;
    // we start the machine
    vA = _mm_lddqu_si128((const __m128i* )array1 + pos1);
    pos1++;
    vB = _mm_lddqu_si128((const __m128i* )array2 + pos2);
    pos2++;
    uint32_t curA, curB;
    curA = (pos1 < len1) ? array1[4*pos1] : 0;
    curB = (pos2 < len2) ? array2[4*pos2] : 0;
    sse_merge(&vA,&vB,&vMin,&vMax);
    laststore = _mm_set1_epi32(array1[0] < array2[0] ? array1[0] -1 : array2[0] -1);
    output+= store_unique(laststore,vMin, output);
    laststore=vMin;
    ///outpos++;
    while((pos1 < len1) && (pos2<len2)) {
        // code is not optimal, too much branching?
        if(curA <= curB) {
            V = _mm_lddqu_si128((const __m128i* )array1 + pos1);
            pos1++;
            curA = (pos1 < len1) ? array1[4*pos1] : 0;
        } else {
            V = _mm_lddqu_si128((const __m128i* )array2 + pos2);
            pos2++;
            curB = (pos2 < len2) ? array2[4*pos2] : 0;
        }
        sse_merge(&V,&vMax,&vMin,&vMax);
        output+= store_unique(laststore,vMin, output);
        laststore = vMin;
    }
    while(pos1<len1) {
        V = _mm_lddqu_si128((const __m128i* )array1 + pos1);
        pos1++;
        sse_merge(&V,&vMax,&vMin,&vMax);
        output+= store_unique(laststore,vMin, output);
        laststore = vMin;
    }
    while(pos2<len2) {
        V = _mm_lddqu_si128((const __m128i* )array2 + pos2);
        pos2++;
        sse_merge(&V,&vMax,&vMin,&vMax);
        output+= store_unique(laststore,vMin, output);
        laststore = vMin;
    }
    output+= store_unique(laststore,vMax,output);

    uint32_t len = output-initoutput  /*outpos * 4*/;

    return len;
}

uint32_t union_uint32(const uint32_t *set_1, uint32_t size_1, const uint32_t *set_2,
                      uint32_t size_2, uint32_t *buffer) {
    uint32_t pos = 0, idx_1 = 0, idx_2 = 0;

    if (0 == size_2) {
        memcpy(buffer, set_1, size_1 * sizeof(uint32_t));
        return size_1;
    }
    if (0 == size_1) {
        memcpy(buffer, set_2, size_2 * sizeof(uint32_t));
        return size_2;
    }

    uint32_t val_1 = set_1[idx_1], val_2 = set_2[idx_2];

    while (true) {
        if (val_1 < val_2) {
            buffer[pos++] = val_1;
            ++idx_1;
            if (idx_1 >= size_1) break;
            val_1 = set_1[idx_1];
        } else if (val_2 < val_1) {
            buffer[pos++] = val_2;
            ++idx_2;
            if (idx_2 >= size_2) break;
            val_2 = set_2[idx_2];
        } else {
            buffer[pos++] = val_1;
            ++idx_1;
            ++idx_2;
            if (idx_1 >= size_1 || idx_2 >= size_2) break;
            val_1 = set_1[idx_1];
            val_2 = set_2[idx_2];
        }
    }
    if (idx_1 < size_1) {
        const uint32_t n_elems = size_1 - idx_1;
        memcpy(buffer + pos, set_1 + idx_1, n_elems * sizeof(uint32_t));
        pos += n_elems;
    } else if (idx_2 < size_2) {
        const uint32_t n_elems = size_2 - idx_2;
        memcpy(buffer + pos, set_2 + idx_2, n_elems * sizeof(uint32_t));
        pos += n_elems;
    }
    return pos;
}

void demo() {
    uint32_t array1[]= {0,1,6,7};
    uint32_t array2[]= {2,3,4,5};
    __m128i vA = _mm_lddqu_si128((const __m128i* )array1);
    __m128i vB = _mm_lddqu_si128((const __m128i* )array2);
    __m128i vMin;
    __m128i vMax;
    sse_merge(&vA,&vB,&vMin,&vMax);
    uint32_t * output = malloc(1024);
    _mm_storeu_si128((__m128i* )output,vMin);
    _mm_storeu_si128((__m128i* )output+1,vMax);
    for(int k = 0 ; k < 8; ++k) {
        printf("%u ", output[k]);
    }
    free(output);
    printf("\n");
}


void skewedbigdemo(uint32_t len1, uint32_t len2) {
    assert(len1 % 4 == 0);
    uint32_t * array1= malloc(len1 * sizeof(uint32_t));
    assert(len2 % 4 == 0);
    uint32_t * array2= malloc(len2 * sizeof(uint32_t));
    uint32_t last;
    last = 0;
    for(int k = 0; k < len1; ++k) {
        last = last + (rand()&0x0F) + 1;
        array1[k] =last;
    }
    assert(array_is_strictly_sorted(array1,len1));

    last = 0;
    for(int k = 0; k < len2; ++k) {
        last = last + (rand()&0x0F) + 1;
        array2[k] = last;
    }
    assert(array_is_strictly_sorted(array2,len2));

    uint32_t * output = malloc((len1 + len2) * sizeof(uint32_t));
    uint32_t len = sse_unite(array1, len1,array2, len2,output);
    for(int k = 0 ; k < len; ++k) {
        if(k>0) {
            if(output[k]<=output[k-1]) {
                printf("bug at k =%d out of %d:  %u %u",k,len,output[k-1],output[k]);
                abort();
            }
        }
    }
    uint32_t * outputs = malloc((len1 + len2) * sizeof(uint32_t));
    uint32_t lens = sse_unite_simple(array1, len1,array2, len2,outputs);
    for(int k = 0 ; k < lens; ++k) {
        if(k>0) {
            if(outputs[k]<=outputs[k-1]) {
                printf("bug at k =%d out of %d:  %u %u",k,lens,outputs[k-1],outputs[k]);
                abort();
            }
        }
    }


    uint32_t * output2 = malloc((len1 + len2) * sizeof(uint32_t));

    uint32_t lenslow =  union_uint32(array1, len1,array2, len2,output2);
    if(len != lenslow) {
        printf("bad length = %d %d \n", len,lenslow);
        array_print(output,len);
        array_print(output2,lenslow);
        abort();
    }
    if(lens != lenslow) {
        printf("bad s length = %d %d \n", lens,lenslow);
        array_print(outputs,lens);
        array_print(output2,lenslow);
        abort();
    }
    for(int k = 0 ; k < len; ++k) {
        if(output[k]!=output2[k]) {
            printf("bug at k =%d out of %d:  %u %u",k,len,output[k],output2[k]);
            abort();
        }
        if(outputs[k]!=output2[k]) {
            printf("bug at k =%d out of %d:  %u %u",k,len,outputs[k],output2[k]);
            abort();
        }

    }
    free(output2);
    free(outputs);
    free(output);
    free(array1);
    free(array2);
}


void benchmark(uint32_t size) {
    printf("== benchmark size = %d \n", size);
    assert(size % 4 == 0);
    uint32_t * array1= malloc(size * sizeof(uint32_t));
    uint32_t len1 = size;
    uint32_t * array2= malloc(size * sizeof(uint32_t));
    uint32_t len2 = size;
    uint32_t last;
    last = 0;
    for(int k = 0; k < len1; ++k) {
        last = last + (rand()&0x0F) + 1;
        array1[k] =last;
    }
    assert(array_is_strictly_sorted(array1,len1));

    last = 0;
    for(int k = 0; k < len2; ++k) {
        last = last + (rand()&0x0F) + 1;
        array2[k] = last;
    }
    assert(array_is_strictly_sorted(array2,len2));

    uint32_t * output = malloc(2 * size * sizeof(uint32_t));
    uint64_t cycles_start = 0, cycles_final = 0;
    RDTSC_START(cycles_start);
    uint32_t lensse = sse_unite(array1, len1,array2, len2,output);
    RDTSC_FINAL(cycles_final);
    printf("SSE took %" PRIu64 " cycles\n",
           cycles_final - cycles_start);
    RDTSC_START(cycles_start);
    uint32_t slensse = sse_unite_simple(array1, len1,array2, len2,output);
    RDTSC_FINAL(cycles_final);
    printf("simple SSE took %" PRIu64 " cycles\n",
           cycles_final - cycles_start);

    RDTSC_START(cycles_start);
    uint32_t lenslow =  union_uint32(array1, len1,array2, len2,output);
    RDTSC_FINAL(cycles_final);
    printf("scalar took %" PRIu64 " cycles\n",
           cycles_final - cycles_start);
    printf("lensse = %d slensse = %d lenslow = %d \n", lensse, slensse,lenslow);
    assert(lensse == lenslow);
    assert(slensse == lenslow);


    free(output);
    free(array1);
    free(array2);
}

int main() {
    for(int k = 4; k <32; k+=4) {
        printf("k=%d\n",k);
        for(int j = 0; j< 1000; ++j) {
            skewedbigdemo(k, k);
        }
        for(int j = 0; j< 1000; ++j) {
            skewedbigdemo(k, 2*k);
        }
        for(int j = 0; j< 1000; ++j) {
            skewedbigdemo(2*k, k);
        }
    }
    benchmark(512);
    benchmark(1024);
    benchmark(2048);

}
