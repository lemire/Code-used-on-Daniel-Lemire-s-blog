#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct fastdiv_s {
  int shift;
  uint64_t inverse;
};
typedef struct fastdiv_s fastdiv_t;

// using C11
fastdiv_t precomputed[65] = {
    {0, 0},
    {0, 0x1},
    {1, 0x1},
    {0, 0xaaaaaaaaaaaaaaab},
    {2, 0x1},
    {0, 0xcccccccccccccccd},
    {1, 0xaaaaaaaaaaaaaaab},
    {0, 0x6db6db6db6db6db7},
    {3, 0x1},
    {0, 0x8e38e38e38e38e39},
    {1, 0xcccccccccccccccd},
    {0, 0x2e8ba2e8ba2e8ba3},
    {2, 0xaaaaaaaaaaaaaaab},
    {0, 0x4ec4ec4ec4ec4ec5},
    {1, 0x6db6db6db6db6db7},
    {0, 0xeeeeeeeeeeeeeeef},
    {4, 0x1},
    {0, 0xf0f0f0f0f0f0f0f1},
    {1, 0x8e38e38e38e38e39},
    {0, 0x86bca1af286bca1b},
    {2, 0xcccccccccccccccd},
    {0, 0xcf3cf3cf3cf3cf3d},
    {1, 0x2e8ba2e8ba2e8ba3},
    {0, 0xd37a6f4de9bd37a7},
    {3, 0xaaaaaaaaaaaaaaab},
    {0, 0x8f5c28f5c28f5c29},
    {1, 0x4ec4ec4ec4ec4ec5},
    {0, 0x84bda12f684bda13},
    {2, 0x6db6db6db6db6db7},
    {0, 0x34f72c234f72c235},
    {1, 0xeeeeeeeeeeeeeeef},
    {0, 0xef7bdef7bdef7bdf},
    {5, 0x1},
    {0, 0xf83e0f83e0f83e1},
    {1, 0xf0f0f0f0f0f0f0f1},
    {0, 0xaf8af8af8af8af8b},
    {2, 0x8e38e38e38e38e39},
    {0, 0x14c1bacf914c1bad},
    {1, 0x86bca1af286bca1b},
    {0, 0x6f96f96f96f96f97},
    {3, 0xcccccccccccccccd},
    {0, 0x8f9c18f9c18f9c19},
    {1, 0xcf3cf3cf3cf3cf3d},
    {0, 0x82fa0be82fa0be83},
    {2, 0x2e8ba2e8ba2e8ba3},
    {0, 0x4fa4fa4fa4fa4fa5},
    {1, 0xd37a6f4de9bd37a7},
    {0, 0x51b3bea3677d46cf},
    {4, 0xaaaaaaaaaaaaaaab},
    {0, 0x7d6343eb1a1f58d1},
    {1, 0x8f5c28f5c28f5c29},
    {0, 0xfafafafafafafafb},
    {2, 0x4ec4ec4ec4ec4ec5},
    {0, 0x21cfb2b78c13521d},
    {1, 0x84bda12f684bda13},
    {0, 0x6fb586fb586fb587},
    {3, 0x6db6db6db6db6db7},
    {0, 0x823ee08fb823ee09},
    {1, 0x34f72c234f72c235},
    {0, 0xcbeea4e1a08ad8f3},
    {2, 0xeeeeeeeeeeeeeeef},
    {0, 0x4fbcda3ac10c9715},
    {1, 0xef7bdef7bdef7bdf},
    {0, 0xefbefbefbefbefbf},
};

int safen[65] = {0,       INT_MAX,
                 2642246, 77936,
                 10206,   2762,
                 1122,    585,
                 359,     247,
                 184,     146,
                 121,     104,
                 92,      83,
                 77,      72,
                 68,      65,
                 63,      61,
                 59,      58,
                 58,      57,
                 57,      56,
                 56,      56,
                 56,      56,
                 57,      57,
                 57,      58,
                 58,      58,
                 59,      59,
                 60,      61,
                 61,      62,
                 62,      62,
                 62,      62,
                 62,      62,
                 62,      62,
                 62,      62,
                 62,      62,
                 62,      62,
                 62,      62,
                 62,      62,
                 62,      62,
                 62};

// correct for n <= 70, k <= 8
uint64_t
fastbinomial(int n, int k) {
  // we should validate the values of n and k, obviously
  //  if( (k <= 0) || (n <= 0)) {
  //      bail out
  //
  //  }
  //  if( k > 64) {
  //      bail out
  //  }
  //  if (n > safen[k]) {
  //      bail out
  //  }
  uint64_t np = n - k;
  uint64_t answer = np + 1;
  for (uint64_t z = 2; z <= (uint64_t)k; z++) {
    answer = answer * (np + z); // this could overflow! but it won't for our
                                // range of values for n and k: require that n <= safen[k]
    fastdiv_t f = precomputed[z];
    answer >>= f.shift;
    answer *= f.inverse;
  }
  return answer;
}

bool test(int maxn, int maxk) {

  for (int n = 0; n <= maxn; n++) {
    if(fastbinomial(n, 1) != n) {
        printf("problem with k = 1\n");
        return false;
    }
  }

  for (int k = 2; k <= maxk; k++) {
    for (int n = k; n <= maxn; n++) {
        if(fastbinomial(n, k) != fastbinomial(n-1, k)+fastbinomial(n-1, k-1)) {
            printf("problem with Pascal's rule at k = %d n = %d\n", k, n);
            printf("%d %d %d\n", fastbinomial(n, k), fastbinomial(n-1, k),fastbinomial(n-1, k-1));
            return false;
        }
    }
  }
  return true;
}


int main() {
  for (int k = 2; k <= 64; k++) {
      printf("k = %d\n",k);
      if(!test(safen[k],k)) {printf("bug\n");}
  }
}