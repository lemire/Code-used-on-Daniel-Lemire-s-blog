// gcc -march=native -O3 -o simdunion simdunion.c
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <assert.h>

#include <x86intrin.h>

// Inoue and Taura, SIMD- and Cache-Friendly Algorithm for Sorting an Array of Structures
static void sse_merge(__m128i *vA, __m128i *vB, // input 1 & 2
__m128i *vMin, __m128i *vMax) { // output
__m128i vTmp; // temporary register

//vA = 0 1 6 7 (hypothetical)
//vB = 2 3 4 5 (hypothetical)

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
// result: 0 1 2 3
}


static uint32_t sse_unite(uint32_t * __restrict__ array1, uint32_t length1 ,
    uint32_t *  __restrict__ array2, uint32_t length2, uint32_t *  __restrict__  output) {
    __m128i vA, vB, vMin,vMax,vOld;
    assert(length1 % 4 == 0); // because I am lazy
    assert(length2 % 4 == 0); // because I am lazy
    assert(length1 != 0); // because I am lazy
    assert(length2 != 0); // because I am lazy

    uint32_t len1 = length1 / 4;
    uint32_t len2 = length2 / 4;
    uint32_t pos1 = 0;
    uint32_t pos2 = 0;
    uint32_t outpos = 0;

    // we start the machine
    vA = _mm_lddqu_si128((const __m128i* )array1 + pos1);
    pos1++;
    vB = _mm_lddqu_si128((const __m128i* )array2 + pos2);
    pos2++;
    sse_merge(&vA,&vB,&vMin,&vOld);
    _mm_storeu_si128((__m128i* )output + outpos,vMin);
    outpos++;
    while(true) {
        vA = _mm_lddqu_si128((const __m128i* )array1 + pos1);
        vB = _mm_lddqu_si128((const __m128i* )array2 + pos2);
        sse_merge(&vA,&vB,&vMin,&vMax);
        _mm_storeu_si128((__m128i* )output + outpos,vMin);
        outpos++;
        sse_merge(&vMax,&vOld,&vMin,&vOld);
        _mm_storeu_si128((__m128i* )output + outpos,vMin);
        outpos++;
    }
    _mm_storeu_si128((__m128i* )output + outpos,vOld);
    outpos++;
    return outpos * 4;
}

void demo() {
    uint32_t array1[]={0,1,6,7};
    uint32_t array2[]={2,3,4,5};
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


int main() {
  uint32_t array1[]={0,1,6,7};
  uint32_t len1 = 4;
  uint32_t array2[]={2,3,4,5};
  uint32_t len2 = 4;
  uint32_t * output = malloc(1024);
  uint32_t len = sse_unite(array1, len1,array2, len2,output);
  for(int k = 0 ; k < len; ++k) {
    printf("%u ", output[k]);
  }
  printf("\n");
  free(output);
  return 0;
}
