/**
* cost of remainder in 64-bit multiplications
g++ -O2 -march=native -o costofremainder costofremainder.cpp && ./costofremainder

g++ -O2 -march=native -o costofremainder costofremainder.cpp -DIACA 
/opt/intel/iaca/bin/iaca.sh -64 -mark 1 ./costofremainder
/opt/intel/iaca/bin/iaca.sh -64 -mark 2 ./costofremainder
/opt/intel/iaca/bin/iaca.sh -64 -mark 3 ./costofremainder

*/
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <stdint.h>
#include <x86intrin.h>
using namespace std;

#ifdef IACA
#include </opt/intel/iaca/include/iacaMarks.h>
#endif

#define MUL64(rh,rl,i1,i2) asm ("mulq %3" : "=a"(rl), "=d"(rh) : "a"(i1), "r"(i2) : "cc")
#define ADD128(rh,rl,ih,il)                                               \
    asm ("addq %3, %1 \n\t"                                               \
         "adcq %2, %0"                                                    \
    : "+r"(rh),"+r"(rl)                                                   \
    : "r"(ih),"r"(il) : "cc");



struct uint192 {
    uint64_t low;
    uint64_t high;
    uint64_t vhigh;
} ;

void completesum(const uint64_t* a, const uint64_t *  x, const size_t length, uint64_t * out) {
    uint192 s;
    s.low = 0;
    s.high = 0;
    s.vhigh = 0;
    size_t i = 0;
    for(; i<length*8/8; i+= 8) {
#ifdef IACA
        IACA_START;
#endif
        __asm__ (
            "movq (%[u]),%%rax\n"
            "mulq (%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 8(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 8(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 16(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 16(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 24(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 24(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 32(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 32(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 40(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 40(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 48(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 48(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 56(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 56(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            :  [rh] "+r" (s.high), [rhh] "+r" (s.vhigh) , [rl] "+r" (s.low)  : [u] "r" (a+i), [v] "r" (x+i)  :"rdx","rax","memory","cc");
#ifdef IACA
        IACA_END;
#endif
    }
    for(; i<length; ++i) {
        __asm__ ("mulq %[v]\n"
                 "addq %%rax,  %[rl]\n"
                 "adcq %%rdx,  %[rh]\n"
                 "adcq $0,  %[rhh]\n"
                 :  [rh] "+r" (s.high), [rhh] "+r" (s.vhigh) , [rl] "+r" (s.low)  : [u] "a" (a[i]), [v] "r" (x[i])  :"rdx","cc");
    }
    out[0] = s.low;
    out[1] = s.high;
    out[2] = s.vhigh;
}


// like MHH, this is essentially multilinear with 64bit multiplication
// summed up over a 128-bit counter
void MMHsum(const uint64_t* randomsource, const uint64_t *  string, const size_t length, uint64_t * out) {

    uint64_t low = 0;
    uint64_t high = 0;
    size_t i = 0;
    for(; i<length/8*8; i+=8) {
#ifdef IACA
        IACA_START;
#endif
        __asm__ (
            "movq (%[u]),%%rax\n"
            "mulq (%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 8(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "mulq 8(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 16(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "mulq 16(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 24(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "mulq 24(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 32(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "mulq 32(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 40(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "mulq 40(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 48(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "mulq 48(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 56(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "mulq 56(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "adcq %%rdx,  %[rh]\n"
            :  [rh] "+r" (high) , [rl] "+r" (low)  : [u] "r" (randomsource+i), [v] "r" (string+i)  :"rdx","rax","memory","cc");
#ifdef IACA
        IACA_END;
#endif
    }

    for(; i<length; ++i) {
        __asm__ ("mulq %[v]\n"
                 "addq %%rax,  %[rl]\n"
                 "adcq %%rdx,  %[rh]\n"
                 :  [rh] "+r" (high), [rl] "+r" (low)  : [u] "a" (randomsource[i]), [v] "r" (string[i])  :"rdx","cc");
    }
    out[0] = low;
    out[1] = high;
}


// a sum with half the number of multiplications
void NHsum(const uint64_t* kp, const uint64_t *  mp, const size_t length, uint64_t * out) {
    uint64_t th, tl;
    uint64_t rh = 0;
    uint64_t rl = 0;
    uint64_t low = 0;
    uint64_t high = 0;
    size_t i = 0;
    for(; i<length/8*8; i+=8) {
#ifdef IACA
        IACA_START;
#endif
        MUL64(th,tl,mp[i  ]+(kp)[i  ],mp[i +1 ]+(kp)[i+1]);
        ADD128(rh,rl,th,tl);
        MUL64(th,tl,mp[i+2]+(kp)[i+2],mp[i +3 ]+(kp)[i+3]);
        ADD128(rh,rl,th,tl);
        MUL64(th,tl,mp[i +4 ]+(kp)[i+4],mp[i +5 ]+(kp)[i+5]);
        ADD128(rh,rl,th,tl);
        MUL64(th,tl,mp[i +6 ]+(kp)[i+6],mp[i +7 ]+(kp)[i+7]);
        ADD128(rh,rl,th,tl);
#ifdef IACA
        IACA_END;
#endif
    }
    if(i!=length) cout<<"please use a multiple of 8"<<endl;
    out[0] = rl;
    out[1] = rh;
}


void completesum2(const uint64_t* a, const uint64_t *  x, const size_t length, uint64_t * out) {
    uint192 s;
    s.low = 0;
    s.high = 0;
    s.vhigh = 0;
    size_t i = 0;
    for(; i<length*8/8; i+= 8) {
#ifdef IACA
        IACA_START;
#endif
        __asm__ (
            "movq (%[u]),%%rax\n"
            "mulq (%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 8(%[u]),%%rax\n"
            "addq %%rdx,  %[rh]\n"
            "addq $0,  %[rhh]\n"
            "mulq 8(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 16(%[u]),%%rax\n"
            "addq %%rdx,  %[rh]\n"
            "addq $0,  %[rhh]\n"
            "mulq 16(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 24(%[u]),%%rax\n"
            "addq %%rdx,  %[rh]\n"
            "addq $0,  %[rhh]\n"
            "mulq 24(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 32(%[u]),%%rax\n"
            "addq %%rdx,  %[rh]\n"
            "addq $0,  %[rhh]\n"
            "mulq 32(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 40(%[u]),%%rax\n"
            "addq %%rdx,  %[rh]\n"
            "addq $0,  %[rhh]\n"
            "mulq 40(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 48(%[u]),%%rax\n"
            "addq %%rdx,  %[rh]\n"
            "addq $0,  %[rhh]\n"
            "mulq 48(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 56(%[u]),%%rax\n"
            "addq %%rdx,  %[rh]\n"
            "addq $0,  %[rhh]\n"
            "mulq 56(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "addq %%rdx,  %[rh]\n"
            "addq $0,  %[rhh]\n"
                        :  [rh] "+r" (s.high), [rhh] "+r" (s.vhigh) , [rl] "+r" (s.low)  : [u] "r" (a+i), [v] "r" (x+i)  :"rdx","rax","memory","cc");
#ifdef IACA
        IACA_END;
#endif
    }
    for(; i<length; ++i) {
        __asm__ ("mulq %[v]\n"
                 "addq %%rax,  %[rl]\n"
                 "adcq %%rdx,  %[rh]\n"
                 "adcq $0,  %[rhh]\n"
                 :  [rh] "+r" (s.high), [rhh] "+r" (s.vhigh) , [rl] "+r" (s.low)  : [u] "a" (a[i]), [v] "r" (x[i])  :"rdx","cc");
    }
    out[0] = s.low;
    out[1] = s.high;
    out[2] = s.vhigh;
}



void completesumi(const uint64_t* a, const uint64_t *  x, const size_t length, uint64_t * out) {
    uint192 s;
    s.low = 0;
    s.high = 0;
    s.vhigh = 0;
    uint192 ss;
    ss.low = 0;
    ss.high = 0;
    ss.vhigh = 0;

    size_t i = 0;
    for(; i<length*8/8; i+= 8) {
#ifdef IACA
        IACA_START;
#endif
        __asm__ (
            "movq (%[u]),%%rax\n"
            "mulq (%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 8(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 8(%[v])\n"
            "addq %%rax,  %[rrl]\n"
            "movq 16(%[u]),%%rax\n"
            "adcq %%rdx,  %[rrh]\n"
            "adcq $0,  %[rrhh]\n"
            "mulq 16(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 24(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 24(%[v])\n"
            "addq %%rax,  %[rrl]\n"
            "movq 32(%[u]),%%rax\n"
            "adcq %%rdx,  %[rrh]\n"
            "adcq $0,  %[rrhh]\n"
            "mulq 32(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 40(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 40(%[v])\n"
            "addq %%rax,  %[rrl]\n"
            "movq 48(%[u]),%%rax\n"
            "adcq %%rdx,  %[rrh]\n"
            "adcq $0,  %[rrhh]\n"
            "mulq 48(%[v])\n"
            "addq %%rax,  %[rl]\n"
            "movq 56(%[u]),%%rax\n"
            "adcq %%rdx,  %[rh]\n"
            "adcq $0,  %[rhh]\n"
            "mulq 56(%[v])\n"
            "addq %%rax,  %[rrl]\n"
            "adcq %%rdx,  %[rrh]\n"
            "adcq $0,  %[rrhh]\n"
            :  [rh] "+r" (s.high), [rhh] "+r" (s.vhigh) , [rl] "+r" (s.low) ,[rrh] "+r" (ss.high), [rrhh] "+r" (ss.vhigh) , [rrl] "+r" (ss.low)  : [u] "r" (a+i), [v] "r" (x+i)  :"rdx","rax","memory","cc");
#ifdef IACA
        IACA_END;
#endif
    }
    
    for(; i<length; ++i) {
        __asm__ ("mulq %[v]\n"
                 "addq %%rax,  %[rl]\n"
                 "adcq %%rdx,  %[rh]\n"
                 "adcq $0,  %[rhh]\n"
                 :  [rh] "+r" (s.high), [rhh] "+r" (s.vhigh) , [rl] "+r" (s.low)  : [u] "a" (a[i]), [v] "r" (x[i])  :"rdx","cc");
    }
    out[0] = s.low;
    out[1] = s.high;
    out[2] = s.vhigh;
}

int main() {
    const size_t N = 100*128;
    uint64_t a[N];
    uint64_t x[N];
    uint64_t out[3];
    for(size_t i = 0; i < N; ++i) {
        a[i] = rand() + (((uint64_t)rand())<<32);
        x[i] = rand() + (((uint64_t)rand())<<32);
    }
    uint192 s1, s2;
    const clock_t S1 = clock();
    out[0]=0; out[1]=0; out[2]=0;   
    for(int T=0; T<10000; ++T) {
        completesum( a, x, N,out);
    }
    cout<<out[0]<<" "<<out[1]<<" "<<out[2]<<endl;
    out[0]=0; out[1]=0; out[2]=0;  
    const clock_t S2 = clock();
    for(int T=0; T<10000; ++T) {
        MMHsum( a, x, N,out);
    }
    cout<<out[0]<<" "<<out[1]<<" "<<out[2]<<endl;
    out[0]=0; out[1]=0; out[2]=0;  

    const clock_t S3 = clock();
    for(int T=0; T<10000; ++T) {
        NHsum( a, x, N,out);
    }
    cout<<out[0]<<" "<<out[1]<<" "<<out[2]<<endl;
    out[0]=0; out[1]=0; out[2]=0;  

    const clock_t S4 = clock();
    for(int T=0; T<10000; ++T) {
        completesumi( a, x, N,out);
    }
    const clock_t S5 = clock();
    cout<<out[0]<<" "<<out[1]<<" "<<out[2]<<endl;
     

    cout<<"complete sum time="<<(double)(S2-S1)/ CLOCKS_PER_SEC<<endl;
    cout<<"MMH sum ="<<(double)(S3-S2)/ CLOCKS_PER_SEC<<endl;
    cout<<"NH sum ="<<(double)(S4-S3)/ CLOCKS_PER_SEC<<endl;
    cout<<"complete sum time="<<(double)(S5-S4)/ CLOCKS_PER_SEC<<endl;

}
