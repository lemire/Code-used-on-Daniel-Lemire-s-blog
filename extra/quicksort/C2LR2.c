// File c:/bsd/rigel/sort/C2LR2.c
// Date: Sat Aug 13 13:22:57 2022
// (C) OntoOO/ Dennis de Champeaux
// Recoding of C2LR.c

#include "D3sort.h"

// swap two elements of an array given pointers to the two elements, p and q
#define PSWAP(p, q) { void *t2 = *(void**)p; *(void**)p = *(void**)q; *(void**)q = t2; }

#define dflgmLimit 250
#define bufSize 200

void cut2lr2c(void **A, int lo, int hi, int depthLimit,
		 int (*compareXY)(const void*, const void*));
/*
void cut2d0c(void **A, int lo, int hi, int depthLimit, 
		    int (*compareXY)(const void*, const void*));
void cut2d0cd(void **A, int lo, int hi, int p, int q, int r, 
	      int depthLimit, int (*compareXY)(const void*, const void*));
*/

void cut2lr2(void **A, int lo, int hi, int (*compare)(const void*, const void*)) {
    int size = hi - lo;
    int depthLimit = 2.9 * floor(log(size));
    if ( size < dflgmLimit ) { 
      // cut2d0c(A, lo, hi, depthLimit, compare);
      dflgm3(A, lo, hi, depthLimit, compare);
      // quicksortmc(A, lo, hi, depthLimit, compare);
      // cut2c(A, lo, hi, depthLimit, compare);
      return;
    }
    // cut2lr2c(A+lo, A+hi, depthLimit, compare);
    cut2lr2c(A, lo, hi, depthLimit, compare);
} // end cut2lr2

void cut2lr2c1(void **A, void **hip, void** bufl[], void** bufr[], 
		      int depthLimit, 
		      int (*compareXY)(const void*, const void*));
// cut2lr2cp used if dflgm is invoked
void cut2lr2c(void **A, int lo, int hi, int depthLimit,
		 int (*compareXY)(const void*, const void*)) {
  void **bufl[bufSize];
  void **bufr[bufSize];
  cut2lr2c1(A+lo, A+hi, bufl, bufr, depthLimit, compareXY);
} // end cut2lr2c

void cut2lr2c1(void **A, void **hip, void** bufl[], void** bufr[], 
		      int depthLimit, 
		      int (*compareXY)(const void*, const void*)) {
 Start:;
  // printf("while cut2lr2c A: %d hip: %d %d \n", A, hip, (hip-A));
  int size = hip-A; 
  if ( size <= 0 ) return;
  if ( depthLimit <= 0 ) {
    heapc(A, 0, size-1, compareXY);
    return;
  }
  depthLimit--; 

  if ( size < dflgmLimit ) {
    // cut2d0c(A, 0, hip-A, depthLimit, compareXY);
    dflgm3(A, 0, hip-A, depthLimit, compareXY);
    return;
  }
  
  // printf("while X cut2k4np A: %d hip: %d %d \n", A, hip, (hip-A));
  
  void **midp = A + (size>>1); // midp points to array midpoint
  
  int numberSamples = (int)sqrt(size/7.0);
  if (numberSamples < 9) numberSamples = 9;
  int halfSegmentLng = numberSamples >> 1; // numberSamples/2;
  void **SampleStart = midp - halfSegmentLng;
  void **SampleEnd = SampleStart + numberSamples -2;
  int offset = size / numberSamples;
  
  // assemble the mini array [SampleStart, SampleEnd)
  // /*
  void **yyp = A;
  void **xxp;
  for( xxp = SampleStart; xxp <= SampleEnd; xxp++) {
    PSWAP(xxp, yyp);
    yyp += offset;
  }
  // */
  /*
  int k;
  for (k = 0; k < numberSamples; k++) 
    { void **xx = SampleStart+k, **yy = A + k*offset; PSWAP(xx, yy); }
  // */
  // sort this mini array to obtain good pivots
  cut2lr2c1(SampleStart, SampleEnd, bufl, bufr, depthLimit, compareXY);
  
  void *T = *midp; // pivot
  if ( compareXY(*SampleStart, T) == 0 ||
       compareXY(*(SampleEnd), T) == 0 ) {
    // pivot has duplicates -> there are likely to be many
    // dflgm is a dutch flag type of algorithm
    dflgm(A, 0, hip-A, midp-A, cut2lr2c, depthLimit, compareXY);
    /*
    PSWAP(A, midp); // make p = A;
    void **r = hip; void **q;
    { void **k; 
      for ( k = SampleEnd; midp < k; k-- ) {
	PSWAP(k, r); r--;
      }
      q = r;  r++; 
      // r is set
      for ( k = midp-1; SampleStart <= k; k-- ) {
	PSWAP(k, q); q--;
      }
      q++;
    }
    cut2d0cd(A, 0, hip-A, 0, q-A, r-A, depthLimit, compareXY);
    // */
    return;
  }
  // printf("C %i\n", (hip-A));

  void **ip = A,  **jp = hip; // indices
  void **kp;

  // Initialize the LEFT partition
  for ( kp = SampleStart; kp <= midp; kp++ ) {
    PSWAP(kp, ip);  ip++;
  }
  ip--;

  // Initialize the RIGHT partition and the PIVOT element
  for ( kp = SampleEnd; midp < kp; kp--) {
    PSWAP(kp, jp);
    jp--;
  }
  jp++;

  /*
       |------]--------------------[------|
       A    ip                    jp    hip
   */
  int kl, kr;
  void **idxl, **idxr; 
  int bufx = bufSize-1;
 Left:
  kl = kr = -1;
  while ( kl < bufx ) {
    while ( compareXY(*++ip, T) <= 0 );
    if ( jp <= ip ) { ip = jp-1; goto MopUpL; }
    bufl[++kl] = ip;
  }
  /*
       |-------*--*---*-]----------[------|
       A               ip          jp    hip
  */

  while ( kr < bufx ) {
    while ( compareXY(T, *--jp) < 0 ); 
    if ( jp <= ip ) { jp = ip+1; goto MopUpR; }
    bufr[++kr] = jp;
  }

  /*
    |-------*--*---*-]----[--*--*-*-------|
    A               ip    jp             hip
  */
  // swap them
 
  while ( 0 <= kl ) {
    idxl = bufl[kl--];
    idxr = bufr[kr--];
    PSWAP(idxl, idxr); 
  }
  goto Left;

 MopUpR:
  // swap them
  while ( 0 <= kr ) {
    idxl = bufl[kl--];
    idxr = bufr[kr--];
    PSWAP(idxl, idxr); 
  }

MopUpL:
  // swap them
  while ( 0 <= kl ) {
    idxl = bufl[kl--];
    if ( jp <= idxl ) continue;
    if ( idxl + 1 == jp ) { jp--; continue; }
    //    { int z = J-1; iswap(z, idxl, A); J = z; }
    jp--; PSWAP(jp,idxl);
  }

  // printf("trace A %i ip %i jp %i hp %i\n", A, ip, jp, hip);
  // Tail iteration
  if ( (ip-A) < (hip-jp) ) { // smallest one first
    cut2lr2c1(A, jp-1, bufl, bufr, depthLimit, compareXY);
    A = jp;
    goto Start;
  }
  cut2lr2c1(jp, hip, bufl, bufr, depthLimit, compareXY);
  hip = jp-1;
  goto Start;
} // end of cut2lr2c

#undef PSWAP
#undef dflgmLimit
#undef bufSize
