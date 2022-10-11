// c:/bsd/rigel/sort/D3sort.c
// Date: Mon Jan 04 18:42:53 2021
// (C) OntoOO/ Dennis de Champeaux

#include "Isort.h"
#include "Hsort.h"
#include "Dsort.h"
#include <math.h>
#define iswap(p, q, A) { void *t3t = A[p]; A[p] = A[q]; A[q] = t3t; }

// calculate the median of 3
static int med(void **A, int a, int b, int c,
	int (*compareXY ) (const void *, const void * ) ) {
  return
    compareXY( A[a], A[b] ) < 0 ?
    ( compareXY( A[b], A[c] ) < 0 ? b : compareXY( A[a], A[c] ) < 0 ? c : a)
    : compareXY( A[b], A[c] ) > 0 ? b : compareXY( A[a], A[c] ) > 0 ? c : a;
} // end med

void dflgm3();
// dflgm0 is used as the driver of a sorter using dflgm3
void dflgm0(void **A, int lo, int hi, int (*compareXY)(const void*, const void*)) {
    // printf("dflgm0 %d %d %d\n", lo, hi, hi-lo);
  int L = hi - lo;
  int depthLimit = 1 + 2.9 * floor(log(L));
  dflgm3(A, lo, hi, depthLimit, compareXY);
}
// extern int icnt; // invocation cnt
void dflgm3(void **A, int lo, int hi, 
           int depthLimit, int (*compareXY)(const void*, const void*)) {
    // Simple version of partitioning with: L/M/R
    // L < pivot, M = pivot, R > pivot
  int L;
  // Printf("dflgm3 lo %i hi %i depthLimit %i\n", lo,hi,depthLimit);
  L = hi - lo +1;
  if ( L <= 1 ) return;

  if ( L < 12 ) { // insertionsort
    // if ( L < 9 ) { // insertionsort
    insertionsort(A, lo, hi, compareXY);
    return;
  }
  if ( depthLimit <= 0 ) {
    heapc(A, lo, hi, compareXY);
    return;
  }
  depthLimit--;
    int p0 = lo + (L>>1); // lo + L/2;
    int px = p0;
    if ( 7 < L ) {
      int pn = lo;
      int pm = hi;
      if ( 40 < L ) {
	int d = (L-2)>>3; // L/8;
	pn = med(A, pn, pn + d, pn + 2 * d, compareXY);
	p0 = med(A, p0 - d, p0, p0 + d, compareXY);
	pm = med(A, pm - 2 * d, pm - d, pm, compareXY);
      }
      p0 = med(A, pn, p0, pm, compareXY);
    }
    if ( p0 != px ) iswap(p0, px, A);
    dflgm(A, lo, hi, px, dflgm3, depthLimit, compareXY);

} // end dflgm3

#undef iswap
