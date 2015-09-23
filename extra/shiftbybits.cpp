/**
*
*  g++ -O2  -o shiftbybits shiftbybits.cpp
*/
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <immintrin.h>
#include <stdio.h>



double mysecond()
{
        struct timeval tp;
        struct timezone tzp;
        int i;

        i = gettimeofday(&tp,&tzp);
        return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

void shiftbybit(uint64_t * in , size_t length,  int b ) {
	//I guess that this would assumes little-endian
  uint64_t tmp = 0;
	for(size_t i = 0; i  < length; ++i) {
		uint64_t val = in[i];
    val = (val << b) | tmp;
    in[i] = val;
    tmp = val >> (64 - b);
	}
}

void manualmove(uint64_t * in , uint64_t * out ,size_t length) {
	//I guess that this would assumes
	for(size_t i = 0; i  < length; ++i) {
		uint64_t val = in[i];
    out[i] = val;
	}
}


void mallocrun(const size_t N) {
    printf(" N = %llu\n",N);
    uint64_t * a = (uint64_t *) malloc(N*sizeof(uint64_t));
    for(size_t k = 0 ; k < N; ++k)
      a[k] = k;
    double timemove = 0;
    double shifttime = 0;
    double timemanualmove = 0;

    for(int k = 0; k<100;++k) {
        double t1 = mysecond();
        memmove(a, a+1, N-1);
        double t2 = mysecond();
        shiftbybit(a,N,1);
        double t3 = mysecond();
        manualmove(a+1, a ,N-1);
        double t4 = mysecond();
        timemove += t2 - t1;
        shifttime += t3 - t2;
        timemanualmove += t4 - t3;
    }
    free(a);
    printf("memmove time = %f, shift time = %f, manual move = %f \n",timemove,shifttime,timemanualmove);
}





int main() {
	mallocrun(10000000);
	return 0;
}
