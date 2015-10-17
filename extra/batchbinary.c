/**
gcc -O3 batchbinary.c -mavx2 -o batchbinary && ./batchbinary
*/
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <x86intrin.h>

size_t branchy_search(int* source, size_t n, int target) {
    size_t lo = 0;
    size_t hi = n;
    while (lo < hi) {
        size_t m = (lo + hi) / 2;
        if (target < source[m]) {
            hi = m;
        } else if (target > source[m]) {
            lo = m+1;
        } else {
            return m;
        }
    }
    return hi;
}



size_t branchfree_search(int* source, size_t n, int target) {
  if((n & (n - 1)) != 0) {
    printf("Warning: this code is only tested for power-of-two inputs.");
  }

    size_t oldn = n;
    int * base = source;
    while(n>1) {
        size_t half = n >> 1;
        base = (base[half] < target) ? &base[half] : base;
        n -= half;
    }
    return ((base < source+oldn)?(*base < target):0) + base - source;
}

void branchfree_search2(int* source, size_t n, int target1, int target2, size_t * index1, size_t * index2) {
  if((n & (n - 1)) != 0) {
    printf("Warning: this code is only tested for power-of-two inputs.");
  }

    int * base1 = source;
    int * base2 = source;
    size_t oldn = n;
    while(n>1) {
        size_t half = n >> 1;
        base1 = (base1[half] < target1) ? &base1[half] : base1;
        base2 = (base2[half] < target2) ? &base2[half] : base2;
        n -= half;
    }
    *index1 = ((base1 < source+oldn)?(*base1 < target1):0) + base1 - source;
    *index2 = ((base2 < source+oldn)?(*base2 < target2):0) + base2 - source;
}

void branchfree_search4(int* source, size_t n, int target1, int target2, int target3, int target4, size_t * index1, size_t * index2, size_t * index3, size_t * index4) {
  if((n & (n - 1)) != 0) {
    printf("Warning: this code is only tested for power-of-two inputs.");
  }

    int * base1 = source;
    int * base2 = source;
    int * base3 = source;
    int * base4 = source;
    size_t oldn = n;
    while(n>1) {
        size_t half = n >> 1;
        base1 = (base1[half] < target1) ? &base1[half] : base1;
        base2 = (base2[half] < target2) ? &base2[half] : base2;
        base3 = (base3[half] < target3) ? &base3[half] : base3;
        base4 = (base4[half] < target4) ? &base4[half] : base4;
        n -= half;
    }
    *index1 = ((base1 < source+oldn)?(*base1 < target1):0) + base1 - source;
    *index2 = ((base2 < source+oldn)?(*base2 < target2):0) + base2 - source;
    *index3 = ((base3 < source+oldn)?(*base3 < target3):0) + base3 - source;
    *index4 = ((base4 < source+oldn)?(*base4 < target4):0) + base4 - source;
}
#ifdef __AVX2__
#define MYAVX
#ifdef MYAVX

void print(__m128i bog) {
    printf("%d %d %d %d \n",_mm_extract_epi32(bog,0),_mm_extract_epi32(bog,1),_mm_extract_epi32(bog,2),_mm_extract_epi32(bog,3));

}
__m128i branchfree_search4_avx(int* source, size_t n, int target1, int target2, int target3, int target4) {
    if((n & (n - 1)) != 0) {
      printf("Warning: this code is only tested for power-of-two inputs.");
    }
    __m128i target = _mm_setr_epi32(target1,target2,target3,target4);
    __m128i offsets = _mm_setzero_si128();
    size_t oldn = n;
    __m128i ha = _mm_set1_epi32(n);
    while(n>1) {
        ha = _mm_srli_epi32(ha,1);
        __m128i offsetsplushalf = _mm_add_epi32(offsets,ha);
        __m128i keys = _mm_i32gather_epi32(source,offsetsplushalf,4);
        __m128i lt = _mm_cmplt_epi32(keys,target);
        offsets = _mm_blendv_epi8(offsets,offsetsplushalf,lt);
        n -= (n >> 1);
    }
    __m128i lastkeys = _mm_i32gather_epi32(source,offsets,4);
    __m128i lastlt = _mm_cmplt_epi32(lastkeys,target);
    __m128i oneswhereneeded = _mm_srli_epi32(lastlt,31);
    __m128i  answer = _mm_add_epi32(offsets,oneswhereneeded);
    return answer;
}
#endif
#endif
void branchfree_search2_prefetch(int* source, size_t n, int target1, int target2, size_t * index1, size_t * index2) {
    int * base1 = source;
    int * base2 = source;
    while(n>1) {
        size_t half = n >> 1;
        __builtin_prefetch(base1+(half>>1),0,0);
        __builtin_prefetch(base1+half+(half>>1),0,0);
        __builtin_prefetch(base2+(half>>1),0,0);
        __builtin_prefetch(base2+half+(half>>1),0,0);
        base1 = (base1[half] < target1) ? &base1[half] : base1;
        base2 = (base2[half] < target2) ? &base2[half] : base2;
        n -= half;
    }
    *index1 = ((base1 < source+n)?(*base1 < target1):0) + base1 - source;
    *index2 = ((base2 < source+n)?(*base2 < target2):0) + base2 - source;
}

void branchfree_search4_prefetch(int* source, size_t n, int target1, int target2, int target3, int target4, size_t * index1, size_t * index2, size_t * index3, size_t * index4) {
    int * base1 = source;
    int * base2 = source;
    int * base3 = source;
    int * base4 = source;
    size_t oldn = n;
    while(n>1) {
        size_t half = n >> 1;
        __builtin_prefetch(base1+(half>>1),0,0);
        __builtin_prefetch(base1+half+(half>>1),0,0);
        __builtin_prefetch(base2+(half>>1),0,0);
        __builtin_prefetch(base2+half+(half>>1),0,0);
        __builtin_prefetch(base3+(half>>1),0,0);
        __builtin_prefetch(base3+half+(half>>1),0,0);
        __builtin_prefetch(base4+(half>>1),0,0);
        __builtin_prefetch(base4+half+(half>>1),0,0);

        base1 = (base1[half] < target1) ? &base1[half] : base1;
        base2 = (base2[half] < target2) ? &base2[half] : base2;
        base3 = (base3[half] < target3) ? &base3[half] : base3;
        base4 = (base4[half] < target4) ? &base4[half] : base4;
        n -= half;
    }
    *index1 = ((base1 < source+oldn)?(*base1 < target1):0) + base1 - source;
    *index2 = ((base2 < source+oldn)?(*base2 < target2):0) + base2 - source;
    *index3 = ((base3 < source+oldn)?(*base3 < target3):0) + base3 - source;
    *index4 = ((base4 < source+oldn)?(*base4 < target4):0) + base4 - source;
}


size_t branchfree_search_prefetch(int* source, size_t n, int target) {
    int * base = source;
    while(n>1) {
        size_t half = n >> 1;
        __builtin_prefetch(base+(half>>1),0,0);
        __builtin_prefetch(base+half+(half>>1),0,0);
        base = (base[half] < target) ? &base[half] : base;
        n -= half;
    }
    return ((base < source+n)?(*base < target):0) + base - source;
}


int compare (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

int demo(size_t N, size_t Nq) {
    int * queries = (int*)malloc(Nq*sizeof(int));
    int * source = (int*)malloc(N*sizeof(int));
    size_t bogus = 0;
    size_t bogus1 = 0;
    size_t bogus2 = 0;
    size_t bogus3 = 0;
    size_t bogus4 = 0;
    __m128i bog = _mm_setzero_si128();
    size_t i, k, ti;
    printf("===============\n");

    printf("array size (N)=%zu,  number of queries (Nq)=%zu...\n",N,Nq);
    printf("preparing data...\n");
    for(i = 0; i < N; ++i) {
        source[i] = rand();
    }
    qsort (source, N, sizeof(int), compare);
    k = 1;
    for(i = 1; i < N; ++i) {
      if(source[i]!=source[k-1]) {
        source[k] = source[i];
        k++;
      }
    }
    N = k;
    printf("\n\n Warning: Branchless code expects a power of two input.\n\n");

    int maxval = source[N-1];
    for(i = 0; i < Nq; ++i) {
        queries[i] = rand()%(maxval+1);
    }
    printf("beginning tests...\n");
    printf("\n");

    for(ti = 0; ti < 3; ++ti) {
        struct timeval t1, t2, t3, t4, t5, t6, t7, t8, t9;

        gettimeofday(&t6, 0);
        for(k = 0; k+1 < Nq; k+=2)
            branchfree_search2_prefetch(source,N,queries[k],queries[k+1],&bogus1,&bogus2);
        gettimeofday(&t1, 0);
        for(k = 0; k < Nq; ++k)
            bogus += branchfree_search(source,N,queries[k]);
        gettimeofday(&t2, 0);
        for(k = 0; k < Nq; ++k)
            bogus += branchy_search(source,N,queries[k]);
        gettimeofday(&t3, 0);
        for(k = 0; k < Nq; ++k)
            bogus += branchfree_search_prefetch(source,N,queries[k]);
        gettimeofday(&t4, 0);
        for(k = 0; k+1 < Nq; k+=2)
            branchfree_search2(source,N,queries[k],queries[k+1],&bogus1,&bogus2);
        gettimeofday(&t5, 0);
        for(k = 0; k+3 < Nq; k+=4)
            branchfree_search4(source,N,queries[k],queries[k+1],queries[k+2],queries[k+3],&bogus1,&bogus2,&bogus3,&bogus4);
        gettimeofday(&t7, 0);
        for(k = 0; k+3 < Nq; k+=4)
            branchfree_search4_prefetch(source,N,queries[k],queries[k+1],queries[k+2],queries[k+3],&bogus1,&bogus2,&bogus3,&bogus4);
        gettimeofday(&t8, 0);
#ifdef MYAVX
        for(k = 0; k+3 < Nq; k+=4)
            bog = _mm_add_epi32(bog,branchfree_search4_avx(source,N,queries[k],queries[k+1],queries[k+2],queries[k+3]));
#endif
        gettimeofday(&t9, 0);



        printf("branchless time=%llu  \n",t2.tv_sec  * 1000ULL * 1000ULL + t2.tv_usec - (t1.tv_sec  * 1000ULL * 1000ULL + t1.tv_usec));
        printf("branchy time=%llu  \n",t3.tv_sec  * 1000ULL * 1000ULL + t3.tv_usec - (t2.tv_sec  * 1000ULL * 1000ULL + t2.tv_usec));
        printf("branchless time with prefetch=%llu \n",t4.tv_sec  * 1000ULL * 1000ULL + t4.tv_usec - (t3.tv_sec  * 1000ULL * 1000ULL + t3.tv_usec));
        printf("branchless interleaved (2) time=%llu  \n",t5.tv_sec  * 1000ULL * 1000ULL + t5.tv_usec - (t4.tv_sec  * 1000ULL * 1000ULL + t4.tv_usec));
        printf("branchless interleaved (2) (prefetch) time=%llu  \n",t1.tv_sec  * 1000ULL * 1000ULL + t1.tv_usec - (t6.tv_sec  * 1000ULL * 1000ULL + t6.tv_usec));
        printf("branchless interleaved (4) time=%llu  \n",t7.tv_sec  * 1000ULL * 1000ULL + t7.tv_usec - (t5.tv_sec  * 1000ULL * 1000ULL + t5.tv_usec));
        printf("branchless interleaved (4) (prefetch) time=%llu  \n",t8.tv_sec  * 1000ULL * 1000ULL + t8.tv_usec - (t7.tv_sec  * 1000ULL * 1000ULL + t7.tv_usec));
#ifdef MYAVX
        printf("branchless interleaved (4) (AVX) time=%llu  \n",t9.tv_sec  * 1000ULL * 1000ULL + t9.tv_usec - (t8.tv_sec  * 1000ULL * 1000ULL + t8.tv_usec));
#endif


        printf("\n");
    }
#ifdef MYAVX
    bogus += _mm_extract_epi32(bog,0);
#endif

    free(source);
    free(queries);
    return (int) bogus+bogus1+bogus2+bogus3+bogus4;

}

int check(size_t N, size_t Nq) {
    int * queries = (int*)malloc(Nq*sizeof(int));
    int * source = (int*)malloc(N*sizeof(int));
    size_t bogus = 0;
    size_t bogus1 = 0;
    size_t bogus2 = 0;
    size_t i, k, ti;
    for(i = 0; i < N; ++i) {
        source[i] = rand();
    }
    qsort (source, N, sizeof(int), compare);
    k = 1;
    for(i = 1; i < N; ++i) {
      if(source[i]!=source[k-1]) {
        source[k] = source[i];
        k++;
      }
    }
    N = k;
    int maxval = source[N-1];
    for(i = 0; i < Nq; ++i) {
        queries[i] = rand()%(maxval+1);
    }
    for(k = 0; k < Nq; ++k)
           if(branchy_search(source,N,queries[k]) != branchfree_search(source,N,queries[k])) {
             printf("bug1\n");
             free(source);
             free(queries);

             return -1;
    }
    for(k = 0; k+1 < Nq; k+=2) {
        size_t i1, i2;
        branchfree_search2(source,N,queries[k],queries[k+1],&i1,&i2);
        if(branchfree_search(source,N,queries[k]) != i1) {
            printf("bug2\n");
            free(source);
            free(queries);

            return -1;
        }
        if(branchfree_search(source,N,queries[k+1]) != i2) {
            printf("bug3\n");
            free(source);
            free(queries);

            return -1;
        }
    }
#ifdef MYAVX

    for(k = 0; k+3 < Nq; k+=4) {
        size_t i1, i2, i3, i4;
        __m128i bog = branchfree_search4_avx(source,N,queries[k],queries[k+1],queries[k+2],queries[k+3]);
        branchfree_search4(source,N,queries[k],queries[k+1],queries[k+2],queries[k+3],&i1,&i2,&i3,&i4);
        if((_mm_extract_epi32(bog,0)!= i1) || (_mm_extract_epi32(bog,1)!= i2) || (_mm_extract_epi32(bog,2)!= i3) || (_mm_extract_epi32(bog,3)!= i4)) {
            printf("bug3\n");
            printf("%d %d %d %d\n",i1,i2,i3,i4);
            printf("%d %d %d %d\n",_mm_extract_epi32(bog,0),_mm_extract_epi32(bog,1),_mm_extract_epi32(bog,2),_mm_extract_epi32(bog,3));
            return -1;
        }
    }
    #endif

    free(source);
    free(queries);
    return 0;
}



int main() {
    if(check(1024 * 1024,1024 * 1024)) return -1;
    demo(1024,1024 * 1024);
    demo(1024 * 1024,1024 * 1024);
    demo(32 * 1024 * 1024,1024 * 1024);

}
