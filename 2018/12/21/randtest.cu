#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define OPENBSD       (1)
#define JAVA          (2)
#define DIVISIONLESS  (3)
#define DIVISIONLESSW (4)
  
#define RANDTEST_ITER            10
#define RANDTEST_THREADS         128
#define RANDTEST_DEFAULT_LEN     10000000
#define RANDTEST_DEFAULT_S       0x123456ULL
#define RANDTEST_DEFAULT_METHOD  OPENBSD

// Macro to catch CUDA errors in CUDA runtime calls
#define CUDA_SAFE_CALL(call)                                          \
do {                                                                  \
    cudaError_t err = call;                                           \
    if (cudaSuccess != err) {                                         \
        fprintf (stderr, "Cuda error in file '%s' in line %i : %s.\n",\
                 __FILE__, __LINE__, cudaGetErrorString(err) );       \
        exit(EXIT_FAILURE);                                           \
    }                                                                 \
} while (0)

// Macro to catch CUDA errors in kernel launches
#define CHECK_LAUNCH_ERROR()                                          \
do {                                                                  \
    /* Check synchronous errors, i.e. pre-launch */                   \
    cudaError_t err = cudaGetLastError();                             \
    if (cudaSuccess != err) {                                         \
        fprintf (stderr, "Cuda error in file '%s' in line %i : %s.\n",\
                 __FILE__, __LINE__, cudaGetErrorString(err) );       \
        exit(EXIT_FAILURE);                                           \
    }                                                                 \
    /* Check asynchronous errors, i.e. kernel failed (ULF) */         \
    err = cudaDeviceSynchronize();                                    \
    if (cudaSuccess != err) {                                         \
        fprintf (stderr, "Cuda error in file '%s' in line %i : %s.\n",\
                 __FILE__, __LINE__, cudaGetErrorString( err) );      \
        exit(EXIT_FAILURE);                                           \
    }                                                                 \
} while (0)

// A routine to give access to a high precision timer on most systems.
#if defined(_WIN32)
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
double second (void)
{
    LARGE_INTEGER t;
    static double oofreq;
    static int checkedForHighResTimer;
    static BOOL hasHighResTimer;

    if (!checkedForHighResTimer) {
        hasHighResTimer = QueryPerformanceFrequency (&t);
        oofreq = 1.0 / (double)t.QuadPart;
        checkedForHighResTimer = 1;
    }
    if (hasHighResTimer) {
        QueryPerformanceCounter (&t);
        return (double)t.QuadPart * oofreq;
    } else {
        return (double)GetTickCount() * 1.0e-3;
    }
}
#elif defined(__linux__) || defined(__APPLE__)
#include <stddef.h>
#include <sys/time.h>
double second (void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec * 1.0e-6;
}
#else
#error unsupported platform
#endif

/*
  Copyright (c) 2017, Norbert Juffa
  All rights reserved.

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions
  are met:

  1. Redistributions of source code must retain the above copyright 
     notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
// multiply two unsigned 64-bit integers into an unsigned 128-bit product
__device__ ulonglong2 umul64wide (uint64_t a, uint64_t b)
{
    ulonglong2 res;
#if (__CUDA_ARCH__ >= 200) && (__CUDA_ARCH__ < 500)
    asm ("{\n\t"
         ".reg .u32 r0, r1, r2, r3, alo, ahi, blo, bhi;\n\t"
         "mov.b64         {alo,ahi}, %2;\n\t"
         "mov.b64         {blo,bhi}, %3;\n\t"
         "mul.lo.u32      r0, alo, blo;\n\t"
         "mul.hi.u32      r1, alo, blo; \n\t"
         "mad.lo.cc.u32   r1, alo, bhi, r1;\n\t"
         "madc.hi.u32     r2, alo, bhi, 0;\n\t"
         "mad.lo.cc.u32   r1, ahi, blo, r1;\n\t"
         "madc.hi.cc.u32  r2, ahi, blo, r2;\n\t"
         "madc.hi.u32     r3, ahi, bhi, 0;\n\t"
         "mad.lo.cc.u32   r2, ahi, bhi, r2;\n\t"
         "addc.u32        r3, r3, 0;\n\t"
         "mov.b64         %0, {r0,r1};\n\t"  
         "mov.b64         %1, {r2,r3};\n\t"
         "}"
         : "=l"(res.x), "=l"(res.y)
         : "l"(a), "l"(b));
#elif __CUDA_ARCH__ >= 500
    asm ("{\n\t"
         ".reg .u32       alo, ahi, blo, bhi, r0, r1, r2, r3;\n\t"
         ".reg .u32       s0, s1, s2, s3, t0, t1, t2, t3;\n\t"
         ".reg .u16       a0, a1, a2, a3, b0, b1, b2, b3;\n\t"
         // split inputs into 16-bit chunks
         "mov.b64         {alo,ahi}, %2;\n\t"
         "mov.b64         {blo,bhi}, %3;\n\t"
         "mov.b32         {a0,a1}, alo;\n\t"
         "mov.b32         {a2,a3}, ahi;\n\t"
         "mov.b32         {b0,b1}, blo;\n\t"
         "mov.b32         {b2,b3}, bhi;\n\t"
         // first partial sum:
         // a3b3.wide  a1b3.wide  a0b2.wide  a0b0.wide
         //     0      a2b2.wide  a1b1.wide  
         //     0      a3b1.wide  a2b0.wide
         "mul.wide.u16    r0, a0, b0;\n\t"
         "mul.wide.u16    r1, a0, b2;\n\t"
         "mul.wide.u16    r2, a1, b3;\n\t"
         "mul.wide.u16    r3, a3, b3;\n\t"
         "mul.wide.u16    t1, a1, b1;\n\t"
         "mul.wide.u16    t2, a2, b2;\n\t"
         "add.cc.u32      r1, r1, t1;\n\t"
         "addc.cc.u32     r2, r2, t2;\n\t"
         "addc.u32        r3, r3, 0;\n\t"
         "mul.wide.u16    t1, a2, b0;\n\t"
         "mul.wide.u16    t2, a3, b1;\n\t"
         "add.cc.u32      r1, r1, t1;\n\t"
         "addc.cc.u32     r2, r2, t2;\n\t"
         "addc.u32        r3, r3, 0;\n\t"
         // second partial sum:
         // a2b3.wide  a0b3.wide  a0b1.wide
         // a3b2.wide  a1b2.wide  a1b0.wide 
         //     0      a2b1.wide
         //     0      a3b0.wide
         "mul.wide.u16    s0, a0, b1;\n\t"
         "mul.wide.u16    s1, a0, b3;\n\t"
         "mul.wide.u16    s2, a2, b3;\n\t"
         "mul.wide.u16    t1, a2, b1;\n\t"
         "add.cc.u32      s1, s1, t1;\n\t"
         "addc.u32        s2, s2, 0;\n\t"
         "mul.wide.u16    t1, a3, b0;\n\t"
         "add.cc.u32      s1, s1, t1;\n\t"
         "addc.u32        s2, s2, 0;\n\t"
         "mul.wide.u16    t0, a1, b0;\n\t"
         "mul.wide.u16    t1, a1, b2;\n\t"
         "mul.wide.u16    t2, a3, b2;\n\t"
         "add.cc.u32      s0, s0, t0;\n\t"
         "addc.cc.u32     s1, s1, t1;\n\t"
         "addc.cc.u32     s2, s2, t2;\n\t"
         "addc.u32        s3, 0, 0;\n\t"
         // offset second partial sum by 16 bits to the left
         "shf.l.clamp.b32 t3, s2, s3, 16;\n\t"
         "shf.l.clamp.b32 t2, s1, s2, 16;\n\t"
         "shf.l.clamp.b32 t1, s0, s1, 16;\n\t"
         "shf.l.clamp.b32 t0,  0, s0, 16;\n\t"
         // add first sum in r{0,1,2,3} to second sum in t{0,1,2,3}
         "add.cc.u32      r0, r0, t0;\n\t"
         "addc.cc.u32     r1, r1, t1;\n\t"
         "addc.cc.u32     r2, r2, t2;\n\t"
         "addc.u32        r3, r3, t3;\n\t"
         // pack outputs
         "mov.b64         %0, {r0,r1};\n\t"
         "mov.b64         %1, {r2,r3};\n\t"
         "}"
         : "=l"(res.x), "=l"(res.y)
         : "l"(a), "l"(b));
#elif __CUDA_ARCH__
#error unsupported __CUDA_ARCH__
#else // avoid warning
    res.x = 0;
    res.y = 0;
#endif
    return res;
}

/* state equations for Marsaglia's KISS64 PRNG */
#define MWC64 (kiss64->t = (kiss64->x << 58) + kiss64->c, \
               kiss64->c = (kiss64->x >> 6), kiss64->x += kiss64->t, \
               kiss64->c += (kiss64->x < kiss64->t), kiss64->x)
#define XSH64 (kiss64->y ^= (kiss64->y << 13), kiss64->y ^= (kiss64->y >> 17), \
               kiss64->y ^= (kiss64->y << 43))
#define CNG64 (kiss64->z = 6906969069ULL * kiss64->z + 1234567ULL)
#define KISS64 (MWC64 + XSH64 + CNG64)

/* KISS64 state */
typedef struct state_tag {
    uint64_t x, c, y, z, t;
} state;

__device__ uint64_t random64 (state *kiss64)
{
    return KISS64;
}

__device__ void seed_random64 (state *kiss64, int seed)
{
   kiss64->x = 1234567890987654321ULL;
   kiss64->c = 123456123456123456ULL;
   kiss64->y = 362436362436362436ULL;
   kiss64->z = 1066149217761810ULL + seed; /* cheesy "parallelization" */
   kiss64->t = 0LL;
   random64 (kiss64);
}

// returns value in [0,s)
__device__ uint64_t openbsd (state *kiss64, uint64_t s) 
{
    uint64_t t = (-s) % s;
    uint64_t x;
    do {
        x = random64 (kiss64);
    } while (x < t);
    return x % s;
}

// returns value in [0,s)
__device__ uint64_t java (state *kiss64, uint64_t s) 
{
    uint64_t x = random64 (kiss64);
    uint64_t r = x % s;
    while (x - r > UINT64_MAX - s + 1) {
        x = random64 (kiss64);
        r = x % s;
    }
    return r;
}

// returns value in [0,s)
__device__ uint64_t nearlydivisionless (state *kiss64, uint64_t s) 
{
    uint64_t x = random64 (kiss64);
    uint64_t h = __umul64hi (x, s);
    uint64_t l = x * s;
    if (l < s) {
        uint64_t t = -s % s;
        while (l < t) {
            x = random64 (kiss64);
            h =__umul64hi (x, s);
            l = x * s;
        }
    }
    return h;
}

// returns value in [0,s)
// like nearlydivisionless(), but using custom function for full product
__device__ uint64_t nearlydivisionless_w (state *kiss64, uint64_t s) 
{
    uint64_t x = random64 (kiss64);
    ulonglong2 m = umul64wide (x, s);
    uint64_t h = m.y;
    uint64_t l = m.x;
    if (l < s) {
        uint64_t t = -s % s;
        while (l < t) {
            x = random64 (kiss64);
            m = umul64wide (x, s);
            h = m.y;
            l = m.x;
        }
    }
    return h;
}

/* generate len 64-bit random numbers in [0,s] and store in random_number[] */
__global__ void randtest (uint64_t *randnum, int len, uint64_t s, int method)
{
    int stride = gridDim.x * blockDim.x;
    int tid = blockDim.x * blockIdx.x + threadIdx.x;

    state kiss64 = {0};
    uint64_t num;

    seed_random64 (&kiss64, tid);

    for (int i = tid; i < len; i += stride) {
        switch (method) {
        case OPENBSD:
            num = openbsd (&kiss64, s);
            break;
        case JAVA:
            num = java (&kiss64, s);
            break;
        case DIVISIONLESS:
            num = nearlydivisionless (&kiss64, s);
            break;
        case DIVISIONLESSW:
            num = nearlydivisionless_w (&kiss64, s);
            break;
        default:
            printf ("*** bad method\n");
            break;
        }
        randnum[i] = num;
    }
}    

struct randtestOpts {
    uint64_t s;
    int len;
    int method;
};

int processArgs (int argc, char *argv[], struct randtestOpts *opts)
{
    int error = 0;
    memset (opts, 0, sizeof(*opts));
    while (argc) {
        if (*argv[0] == '-') {
            switch (*(argv[0]+1)) {
            case 'n':
                opts->len = atol(argv[0]+2);
                break;
            case 's':
                opts->s = atol(argv[0]+2);
                break;
            case 'm':
                opts->method =atol(argv[0]+2);
                if ((opts->method != OPENBSD) &&
                    (opts->method != JAVA) &&
                    (opts->method != DIVISIONLESS) &&
                    (opts->method != DIVISIONLESSW)) {
                    fprintf (stderr, "bad method: %s\n", argv[0]+2);
                    error++;
                }
                break;
            default:
                fprintf (stderr, "Unknown switch '%c%s'\n", '-', argv[0]+1);
                error++;
                break;
            }
        }
        argc--;
        argv++;
    }
    return error;
}
    
int main (int argc, char *argv[])
{
    double start, stop, elapsed, mintime;
    uint64_t *a=0, *d_a = 0;
    int errors;
    struct randtestOpts opts;

    errors = processArgs (argc, argv, &opts);
    if (errors) {
        return EXIT_FAILURE;
    }
    opts.len = (opts.len) ? opts.len : RANDTEST_DEFAULT_LEN;
    opts.s = (opts.s) ? opts.s : RANDTEST_DEFAULT_S;
    opts.method = (opts.method) ? opts.method : RANDTEST_DEFAULT_METHOD;

    /* Allocate memory on host */
    a = (uint64_t *)malloc(sizeof(a[0]) * opts.len);
    if (!a) {
        printf ("host allocation failed\n");
        return EXIT_FAILURE;
    }

    /* Allocate memory on device */
    CUDA_SAFE_CALL (cudaMalloc((void**)&d_a, sizeof(d_a[0]) * opts.len));
    
    /* Initialize device memory */
    CUDA_SAFE_CALL (cudaMemset(d_a, 0x00, sizeof(d_a[0]) * opts.len)); 

    /* Compute execution configuration */
    dim3 dimBlock(RANDTEST_THREADS);
    int threadBlocks = (opts.len + (dimBlock.x - 1)) / dimBlock.x;
    if (threadBlocks > 65520) threadBlocks = 65520;
    dim3 dimGrid(threadBlocks);
    
    printf ("randtest: operating on vectors of %d uint64_t\n", 
            opts.len);
    printf ("randtest: generating random numbers in [0,%llu]\n", opts.s);
    switch (opts.method) {
    case OPENBSD:
        printf ("randtest: range-restricting method is OPENBSD\n");
        break;
    case JAVA:
        printf ("randtest: range-restricting method is JAVA\n");
        break;
    case DIVISIONLESS:
        printf ("randtest: range-restricting method is DIVISIONLESS\n");
        break;
    case DIVISIONLESSW:
        printf ("randtest: range-restricting method is DIVISIONLESS_W\n");
        break;
    default:
        printf ("unsupported range-restricting method\n");
        break;
    }

    printf ("randtest: using %d threads per block, %d blocks\n", 
            dimBlock.x, dimGrid.x);

    mintime = fabs(log(0.0));
    for (int k = 0; k < RANDTEST_ITER; k++) {
        start = second();
        randtest<<<dimGrid,dimBlock>>>(d_a, opts.len, opts.s, opts.method);
        CHECK_LAUNCH_ERROR();
        stop = second();
        elapsed = stop - start;
        if (elapsed < mintime) mintime = elapsed;
    }
    printf ("randtest: mintime = %.3f msec\n",
            1.0e3 * mintime);

    /* Retrieve generated random numbers */
    CUDA_SAFE_CALL (cudaMemcpy (a, d_a, sizeof(d_a[0]) * opts.len, cudaMemcpyDeviceToHost));

    /* Check that generated num,bers are within desired range */
    for (int i = 0; i < opts.len; i++) {
        if (a [i] >= opts.s) {
            printf ("@ i=%d out of range [0, %llu]: %016llx\n", i, opts.s, a[i]);
        }
    }

    /* free memory on host and device */    
    CUDA_SAFE_CALL (cudaFree(d_a));
    free (a);

    return EXIT_SUCCESS;
}
