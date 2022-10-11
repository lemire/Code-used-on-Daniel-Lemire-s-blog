// File: c:/bsd/rigel/sort/Isort
// Date: Thu Jan 30 21:54:12 2014/ Wed Jan 03 19:48:23 2018/ 
//       Mon May 10 11:29:57 2021
// (C) OntoOO/ Dennis de Champeaux

#include "Isort.h"

// #define iswap(p, q, A) { void *t3t = A[p]; A[p] = A[q]; A[q] = t3t; }

/*
void insertionsort(void **A, int lo, int hi, int (*compareXY)()) {
  if ( hi <= lo ) return;
  int i, j;
  for ( i = lo+1; i <= hi; i++ )
     for ( j = i; lo < j && compareXY(A[j-1], A[j]) > 0; j-- ) 
      // iswap(j-1, j, A); 
       { int xx = j-1; iswap(xx, j, A); }
} // end insertionsort
// */ 



// /*
// Nigel version; clean up from DL from Y?
void insertionsort(void **A, int lo, int hi, int (*compareXY)(const void*, const void*)) {
// void insertionsort0(void **A, int lo, int hi, int (*compareXY)()) {
  if ( hi <= lo ) return;
  int size = hi-lo+1, i, k;

        if ((size & 1) == 0) {   // ensure even size
            void *t = A[lo], *u = A[lo+1];
            if (compareXY(t, u) > 0) { A[lo] = u; A[lo+1] = t; }
            i = lo+2;
        } else
            i = lo+1;
        for (; i < hi; ++i) {
            void *fst = A[k = i], *snd = A[++i];
            if (compareXY(fst, snd) > 0) {
                for (; k > lo; --k) {
                    void *x = A[k - 1];
                    if (compareXY(fst, x) >= 0)
                        break;
                    A[k + 1] = x;
                }
                A[k + 1] = fst;
                for (; k > lo; --k) {
                    void *x = A[k - 1];
                    if (compareXY(snd, x) >= 0)
                        break;
                    A[k] = x;
                }
                A[k] = snd;
            } else {
                for (; k > lo; --k) {
                    void *x = A[k - 1];
                    if (compareXY(snd, x) >= 0)
                        break;
                    A[k + 1] = x;
                }
                if (k != i)
                    A[k + 1] = snd;
                for (; k > lo; --k) {
                    void *x = A[k - 1];
                    if (compareXY(fst, x) >= 0)
                        break;
                    A[k] = x;
                }
                if (k != i)
                    A[k] = fst;
            }
        }
} // end  insertionsort
// */
/*
int binarySearch(void **A, void *item, int low, int high, int (*compareXY)()) {
  if (high <= low)
    return compareXY(item, A[low]) > 0 ? low+1 : low;
  int L = high - low;
  int mid = low + (L>>1); // low + L/2;
  int z = compareXY(item, A[mid]);
  if ( 0 == z ) return mid + 1;
  if ( 0 < z ) 
    return binarySearch(A, item, mid + 1, high, compareXY);
  return binarySearch(A, item, low, mid - 1, compareXY);
} 
*/
/*
void insertionsort(void **A, int lo, int hi, int (*compareXY)()) {
  // printf("\n\nin lo %i hi %i\n", lo, hi);
  // competition:           39997   12.97
  int first = lo + 9; 
                         // 39997 13.05 13.08 13.09 12.91 12.96 12.77 12.77
                         //       12.63 ??
  // int first = lo + 8; 
                         // 39962 12.86 12.92 13.15 13.07 13.14 13.05 13.07
  //                              12.81 12.73 12.75 12.68 12.60 12.57 12.44
  // int first = lo + 7; // 39934 13.13 
  // int first = lo + 6; // 39937 13.14
  // int first = lo + 5; // 39946 13.24
  // int first = lo + 4; // 40012 13.28
  // printf("first %i\n", first); 
  if ( hi <= first ) {
    // printf("Do insertionsort0\n");
    insertionsort0(A, lo, hi, compareXY);
    return;
  }
  // printf("IN lo %i hi %i\n", lo, hi);
  int i = hi+1;
  while ( first <= i) i = i-2;
  // printf("i %i\n", i); 
  // sort the small head first
  insertionsort0(A, lo, i-1, compareXY);
  // proceed by inserting pairs in [i:hi]
  //
  //  |------------------]-----------------|
  //  lo                  i               hi
  //
  void *fst, *snd;
  int loc, j, k;
  while ( i < hi ) {
    // printf("\n2i %i\n", i); 
    fst = A[i]; snd = A[i+1];
    // printf("fstX %i sndX %i\n", i, (i+1));
    if ( compareXY(snd, fst) < 0 ) { // swap fst & snd
      // printf("swapping\n");
      void *t = fst; fst = snd; snd = t;
    } 
    k = i-1;
    // printf("k %i\n", k);
    loc = binarySearch(A, snd, lo, k, compareXY);
    // printf("A loc %i\n", loc);
  //
  //  |--------|------------]fs---------------|
  //  lo     loc            ki               hi
  //
    if ( loc < k ) {
      // printf("passing 1\n");
      for (j = k; loc <= k; k--) {
	// printf("Move %i to %i\n", k, k+2);
	A[k+2] = A[k];
      }
      // printf("Set %i\n", loc+1);
      A[loc+1] = snd;
      k = loc - 1;
      goto DoFst;
    }
    if ( loc == i ) {
      // printf("passing 4\n");
      A[i+1] = snd;
      goto DoFst;
    }
    // k == loc) 
    // printf("passing 00\n");
    A[i+1] = A[k]; A[i] = snd;
    k--;
  
  DoFst:
    // take care of fst
    // printf("passing 2 k %i\n", k); 
    loc = binarySearch(A, fst, lo, k, compareXY);
    // printf("B loc %i\n", loc);

    if ( loc < k ) {
      // printf("passing3\n");
      for (j = k; loc <= k; k--) A[k+1] = A[k];
      A[loc] = fst;
      goto Check;
    }  
    if ( loc == k ) {
      // printf("passing5 %i\n", loc);
      A[k+1] = A[k]; // A[k] = fst;
      // goto Check;
    } 
    // loc == i
    A[loc] = fst;

  Check:
    // printf("passing4\n");
    //
    // for ( j = lo+1; j <= i+1; j++ )
    //    if (compareXY(A[j], A[j-1]) < 0 ) {
    //    	printf("Err2 j %i\n", j); exit(0);
    // }
    // 
    // printf("passing5 sorted lo %i (i+1) %i\n", lo, i+1);
    i=i+2;
  }
} // end  insertionsort
// */

// #undef iswap


