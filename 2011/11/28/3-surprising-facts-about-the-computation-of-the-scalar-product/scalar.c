// Scalar product benchmarks
// Daniel Lemire, Nov. 28 2011
// http://lemire.me/blog/
//
// 3 surprising facts about the computation of scalar products
// http://lemire.me/blog/archives/2011/11/28/3-surprising-facts-about-the-computation-of-the-scalar-product/
//
// gcc -funroll-loops -mno-sse2  -O3 -o scalar scalar.c 
// gcc -funroll-loops  -O3 -o scalar scalar.c 

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef unsigned int uint32;
typedef unsigned long long uint64;

uint32 scalarproduct( uint32 *  v1,  uint32 *  v2,  uint32 *  endv1) {
    uint32 sum = 0;
    for(; v1!= endv1; ++v1, ++v2 ) {
        sum+= (*v2 *  *v1);
    }
    return sum;
}


uint32 scalarproduct2by2( uint32 *  v1,  uint32 *  v2,  uint32 *  endv1) {
    uint64 sum = 0;
    for(; v1!= endv1; v1+=2, v2+=2) {
        sum+= (*v2 *  *v1) + (*(v2 + 1) *   *(v1+1));
    }
    return sum;
}

float scalarproductf( float *  v1,  float *  v2,  float *  endv1) {
    float sum = 0;
    for(; v1!= endv1; ++v1, ++v2 ) {
        sum+= (*v2 *  *v1);
    }
    return sum;
}


float scalarproduct2by2f( float *  v1,  float *  v2,  float *  endv1) {
    float sum = 0;
    for(; v1!= endv1; v1+=2, v2+=2) {
        sum+= (*v2 * *v1) + (*(v2 + 1) *  *(v1+1));
    }
    return sum;
}


uint64 scalarproduct64( uint64 *  v1,  uint64 *  v2,  uint64 *  endv1) {
    uint64 sum = 0;
    for(; v1!= endv1; ++v1, ++v2 ) {
        sum+= (*v2 * *v1);
    }
    return sum;
}


uint64 scalarproduct2by264( uint64 *  v1,  uint64 *  v2,  uint64 *  endv1) {
    uint64 sum = 0;
    for(; v1!= endv1; v1+=2, v2+=2) {
        sum+= (*v2 * *v1) + (*(v2 + 1) *  *(v1+1));
    }
    return sum;
}

double scalarproductf64( double *  v1,  double *  v2,  double *  endv1) {
    float sum = 0;
    for(; v1!= endv1; ++v1, ++v2 ) {
        sum+= (*v2 *  *v1);
    }
    return sum;
}


double scalarproduct2by2f64( double *  v1,  double *  v2,  double *  endv1) {
    float sum = 0;
    for(; v1!= endv1; v1+=2, v2+=2) {
        sum+= (*v2 * *v1) + (*(v2 + 1) *  *(v1+1));
    }
    return sum;
}
 
 
#if (defined (__i386__) || defined( __x86_64__ ))

typedef unsigned long long ticks;

static __inline__ ticks start (void) {
  unsigned cycles_low, cycles_high;
  asm volatile ("CPUID\n\t"
		"RDTSC\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
		"%rax", "%rbx", "%rcx", "%rdx");
  return ((ticks)cycles_high << 32) | cycles_low;
}

static __inline__ ticks stop (void) {
  unsigned cycles_low, cycles_high;
  asm volatile("RDTSCP\n\t"
	       "mov %%edx, %0\n\t"
	       "mov %%eax, %1\n\t"
	       "CPUID\n\t": "=r" (cycles_high), "=r" (cycles_low):: "%rax",
	       "%rbx", "%rcx", "%rdx");
  return ((ticks)cycles_high << 32) | cycles_low;
}

#else
#error Unknown architecture
#endif

  
int main(int argc, char **argv) {
	int i,j;
	uint64 sumToFoolCompiler = 0;
	double sumToFoolCompilerf = 0;
	uint64 bef,aft;
	int N = 2048;
    uint32 v1[2*N];
    uint32 v2[2*N];
    int trials = 100000;
    for (i=0; i < 2*N; ++i) {
    	v1[i]= rand();
    	v2[i]= rand();
    }
    bef = start();
    for(j=0; j < trials; ++j)
      sumToFoolCompiler += scalarproduct( &v1[0],&v2[0],&v1[0] + N);
    aft = stop();
    printf("uint32 cycle count per element = %f  / ignore this: %llu \n",(aft-bef)*1.0/(N*trials), sumToFoolCompiler);
    
    bef = start();
    for(j=0; j < trials; ++j)
      sumToFoolCompiler += scalarproduct64( (uint64 *) &v1[0],(uint64 *) &v2[0],(uint64 *) &v1[2*N]);
    aft = stop();
    printf("uint64 cycle count per element = %f  / ignore this: %llu \n",(aft-bef)*1.0/(N*trials), sumToFoolCompiler);

    bef = start();
    for(j=0; j < trials; ++j)
      sumToFoolCompiler += scalarproduct2by2( &v1[0],&v2[0],&v1[0] + N);
    aft = stop();
    printf("uint32 2x2 cycle count per element = %f  / ignore this: %llu \n",(aft-bef)*1.0/(N*trials), sumToFoolCompiler);

    bef = start();
    for(j=0; j < trials; ++j)
      sumToFoolCompiler += scalarproduct2by264( (uint64 *) &v1[0],(uint64 *) &v2[0],(uint64 *)&v1[2*N] );
    aft = stop();
    printf("uint64 2x2 cycle count per element = %f  / ignore this: %llu \n",(aft-bef)*1.0/(N*trials), sumToFoolCompiler);


    bef = start();
    for(j=0; j < trials; ++j)
      sumToFoolCompilerf += scalarproductf((float *) &v1[0],(float *) &v2[0],(float *) &v1[N]);
    aft = stop();
    printf("float cycle count per element = %f  / ignore this: %f \n",(aft-bef)*1.0/(N*trials), sumToFoolCompilerf);

    bef = start();
    for(j=0; j < trials; ++j)
      sumToFoolCompilerf += scalarproduct2by2f( (float *) &v1[0],(float *) &v2[0],(float *) &v1[N]);
    aft = stop();
    printf("float 2x2 cycle count per element = %f  / ignore this: %f \n",(aft-bef)*1.0/(N*trials), sumToFoolCompilerf);



    bef = start();
    for(j=0; j < trials; ++j)
      sumToFoolCompilerf += scalarproductf64((double *) &v1[0],(double *) &v2[0],(double *) &v1[2*N]);
    aft = stop();
    printf("double cycle count per element = %f  / ignore this: %f \n",(aft-bef)*1.0/(N*trials), sumToFoolCompilerf);



    bef = start();
    for(j=0; j < trials; ++j)
      sumToFoolCompilerf += scalarproduct2by2f64( (double *) &v1[0],(double *) &v2[0],(double *) &v1[2*N]);
    aft = stop();
    printf("double 2x2 cycle count per element = %f  / ignore this: %f \n",(aft-bef)*1.0/(N*trials), sumToFoolCompilerf);
    
    

 }





