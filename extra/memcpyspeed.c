/**
gcc -Wall -march=native -O3 memcpyspeed.c -o memcpyspeed
*/
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <x86intrin.h>



typedef unsigned long long ticks;

// Taken from stackoverflow (see http://stackoverflow.com/questions/3830883/cpu-cycle-count-based-profiling-in-c-c-linux-x86-64)
// Can give nonsensical results on multi-core AMD processors.
ticks rdtsc() {
    unsigned int lo, hi;
    asm volatile (
        "cpuid \n" /* serializing */
        "rdtsc"
        : "=a"(lo), "=d"(hi) /* outputs */
        : "a"(0) /* inputs */
        : "%ebx", "%ecx");
    /* clobbers*/
    return ((unsigned long long) lo) | (((unsigned long long) hi) << 32);
}

ticks startRDTSC(void) {
    return rdtsc();
}

ticks stopRDTSCP(void) {
    return rdtsc();
}




int main() {
    size_t N ;
    size_t Max = 4096;

    int * dest = (int*)malloc(Max*sizeof(int));

    for(N=200; N<= Max; N+=16 ) {
        int * source = (int*)malloc(N*sizeof(int));
        memset(source,N%128,N*sizeof(int));
        ticks bef = startRDTSC();
        memcpy(dest,source, N*sizeof(int));
        ticks aft = stopRDTSCP();
        if((source[127]&0xFF) != N%128) printf("bug");
        bef = startRDTSC();
        memcpy(dest,source, N*sizeof(int));
        aft = stopRDTSCP();
        if((source[127]&0xFF) != N%128) printf("bug");
        free(source);

        printf(
                    "# size = %zu CPU cycle/int = %f     \n",N,
                    (aft - bef) * 1.0 / ( N));
        printf("%zu\t%f \n",N, (aft - bef) * 1.0 / ( N));

    }
    free(dest);
    return 0;
}
