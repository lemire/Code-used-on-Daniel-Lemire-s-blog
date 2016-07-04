// Computes the magic number for uint32_t division.
// Max line length is 57, to fit in hacker.book.
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <assert.h>

struct mu {uint32_t M;     // Magic number,
          int a;           // "add" indicator,
          int s;};         // and shift amount.

struct mu magicu(uint32_t d) {
                           // Must have 1 <= d <= 2**32-1.
   int p;
   uint32_t nc, delta, q1, r1, q2, r2;
   struct mu magu;

   magu.a = 0;             // Initialize "add" indicator.
   nc = -1 - (-d)%d;       // uint32_t arithmetic here.
   p = 31;                 // Init. p.
   q1 = 0x80000000/nc;     // Init. q1 = 2**p/nc.
   r1 = 0x80000000 - q1*nc;// Init. r1 = rem(2**p, nc).
   q2 = 0x7FFFFFFF/d;      // Init. q2 = (2**p - 1)/d.
   r2 = 0x7FFFFFFF - q2*d; // Init. r2 = rem(2**p - 1, d).
   do {
      p = p + 1;
      if (r1 >= nc - r1) {
         q1 = 2*q1 + 1;            // Update q1.
         r1 = 2*r1 - nc;}          // Update r1.
      else {
         q1 = 2*q1;
         r1 = 2*r1;}
      if (r2 + 1 >= d - r2) {
         if (q2 >= 0x7FFFFFFF) magu.a = 1;
         q2 = 2*q2 + 1;            // Update q2.
         r2 = 2*r2 + 1 - d;}       // Update r2.
      else {
         if (q2 >= 0x80000000) magu.a = 1;
         q2 = 2*q2;
         r2 = 2*r2 + 1;}
      delta = d - 1 - r2;
   } while (p < 64 &&
           (q1 < delta || (q1 == delta && r1 == 0)));

   magu.M = q2 + 1;        // Magic number
   magu.s = p - 32;        // and shift amount to return
   if(magu.a != 0) magu.s -= 1;
   return magu;            // (magu.a was set above).
}
struct mu magicu2(uint32_t d) {
                           // Must have 1 <= d <= 2**32-1.
   int p;
   uint32_t p32, q, r, delta;
   struct mu magu;
   magu.a = 0;             // Initialize "add" indicator.
   p = 31;                 // Initialize p.
   q = 0x7FFFFFFF/d;       // Initialize q = (2**p - 1)/d.
   r = 0x7FFFFFFF - q*d;   // Init. r = rem(2**p - 1, d).
   do {
      p = p + 1;
      if (p == 32) p32 = 1;     // Set p32 = 2**(p-32).
      else p32 = 2*p32;
      if (r + 1 >= d - r) {
         if (q >= 0x7FFFFFFF) magu.a = 1;
         q = 2*q + 1;           // Update q.
         r = 2*r + 1 - d;       // Update r.
      }
      else {
         if (q >= 0x80000000) magu.a = 1;
         q = 2*q;
         r = 2*r + 1;
      }
      delta = d - 1 - r;
   } while (p < 64 && p32 < delta);
   magu.M = q + 1;         // Magic number and
   magu.s = p - 32;        // shift amount to return
   if(magu.a != 0) magu.s -= 1;
   return magu;            // (magu.a was set above).
}

uint32_t fastdiv(uint32_t val, struct mu * magu) {
  uint32_t q = (magu->M * (uint64_t) val)>>32;
  if(magu->a != 0) {
    q = ((val - q) >> 1) + q;
  }
  return q >> magu->s;
}

void check(uint32_t div) {
   struct mu magu = magicu2(div);
    uint32_t testvals[] = {1,2,4,8,1<<8,1<<16,1<<24,1000,10000,77777777,1098108429,1671277019,1375388624,34656,4050,26693700};
   for(uint32_t itv = 0 ; itv  < sizeof(testvals)/sizeof(uint32_t); itv++) {

     uint32_t tv = testvals[itv];
     uint32_t expected = tv / div;
     uint32_t comp = fastdiv(tv,&magu);
     if(comp != expected) {
       printf("====checking %lu \n",div);
       printf("M = 0x%08x a = %d s = %d\n", magu.M, magu.a, magu.s);
       printf("********* %lu %lu %lu \n", tv,expected,comp);
     }
     assert(comp == expected);
   }

}


int main() {
   struct mu mag;
   uint32_t fact = 1;
   for(uint32_t x = 2; x <= 12; ++x) {
     fact *= x;
     check(fact);
     printf("fact = %lu \n", fact);

     mag = magicu(fact);
     printf("M = 0x%08x a = %d s = %d\n", mag.M, mag.a, mag.s);
     mag = magicu2(fact);
     //printf("M = 0x%08x a = %d s = %d\n", mag.M, mag.a, mag.s);

   }
  fact = 1;
  for(uint32_t x = 2; x <= 12; ++x) {
        fact *= x;
        check(fact);

        mag = magicu(fact);
        printf("{0x%08x, %s, %d},", mag.M, mag.a?"true":"false", mag.s);
        mag = magicu2(fact);
        //printf("M = 0x%08x a = %d s = %d\n", mag.M, mag.a, mag.s);

  }
printf("\n");
   return 0;
}
