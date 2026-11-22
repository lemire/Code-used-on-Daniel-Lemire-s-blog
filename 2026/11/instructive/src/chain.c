#include <stdio.h>
#include <stdint.h>
#include <time.h>
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec*1e9+t.tv_nsec;}
#define N 1024
uint64_t a[N];
// C2's sequence: ins into lane 0 of the same register every time
__attribute__((noinline)) uint64_t ins_same(const uint64_t*p){uint64_t s=0,r;
  for(int i=0;i<N;i++){__asm__ volatile("mov v16.d[0], %1\n cnt v16.8b, v16.8b\n addv b16, v16.8b\n umov %0, v16.d[0]":"=r"(r):"r"(p[i]):"v16");s+=r;}return s;}
// same, but fmov writes the whole register
__attribute__((noinline)) uint64_t fmov_same(const uint64_t*p){uint64_t s=0,r;
  for(int i=0;i<N;i++){__asm__ volatile("fmov d16, %1\n cnt v16.8b, v16.8b\n addv b16, v16.8b\n umov %0, v16.d[0]":"=r"(r):"r"(p[i]):"v16");s+=r;}return s;}
// ins, but alternate registers
__attribute__((noinline)) uint64_t ins_two(const uint64_t*p){uint64_t s=0,r,q;
  for(int i=0;i<N;i+=2){__asm__ volatile("mov v16.d[0], %2\n mov v17.d[0], %3\n cnt v16.8b, v16.8b\n cnt v17.8b, v17.8b\n addv b16, v16.8b\n addv b17, v17.8b\n umov %0, v16.d[0]\n umov %1, v17.d[0]":"=&r"(r),"=&r"(q):"r"(p[i]),"r"(p[i+1]):"v16","v17");s+=r+q;}return s;}
int main(){for(int i=0;i<N;i++)a[i]=i*0x9E3779B97F4A7C15ull;
  const char*n[]={"ins, one register","fmov, one register","ins, two registers"};
  uint64_t(*f[])(const uint64_t*)={ins_same,fmov_same,ins_two};
  for(int k=0;k<3;k++){double best=1e30;uint64_t s=0;for(int r=0;r<200;r++){double t=now();for(int j=0;j<100;j++)s+=f[k](a);double e=now()-t;if(e<best)best=e;}
  printf("%-20s %.3f ns/word (%llu)\n",n[k],best/100/N,(unsigned long long)s);}}
