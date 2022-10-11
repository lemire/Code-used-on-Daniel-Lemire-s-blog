// c:/bsd/rigel/sort/Dsort.c
// Date: Fri Jan 31 13:32:12 2014
// (C) OntoOO/ Dennis de Champeaux

void dflgm(void **A, int lo, int hi, int pivotx,
           void (*cut)(void**, int, int, int,
                       int (*)(const void*, const void*)),
           int depthLimit, int (*compareXY)(const void*, const void*)) {
  /*
    Simple version of partitioning with: L/M/R
    L < pivot, M = pivot, R > pivot
   */
  int i, j, lw, up; // indices
  void* p3; // pivot
  void* x, *y;
  int r; // comparison output 
  // int z; // for tracing

  i = lo; j = hi; lw = pivotx-1; up = pivotx+1;
  int lo2 = lo+1;
    /*
      |---)-----(----)-------(----|
     lo   i     lw  up       j    hi
      
      lo <= i < lw < up < j <= hi
      2 <= up - lw
      lo <= x < i ==> A[x] < p3
      lw < x < up  ==> A[x] = p3
      j < x <= hi & ==> p3 < A[x] 
    */
  x = A[lo]; // roving variable, the wack-a-mole item
  p3 = A[lo] = A[pivotx]; // There IS a middle value somewhere:
  // p3 is temporarily also in A[lo]; will be replaced at the end
  i++; 
  r = compareXY(x, p3);
  // if ( x < p3 ) goto L0;
  if ( r < 0 ) goto L0;
  // if ( p3 < x ) goto R0;
  if ( 0 < r ) goto R0;
  goto ML0;

 L0:  
   /*
      |---)-----(----)-------(----|
     lo   i     lw  up       j    hi
      x -> L 
   */
  if ( lw < i ) { i--;
    goto L1L;
  }
  // i <= lw
  y = A[i];
  r = compareXY(y, p3);
  if ( r < 0 ) { i++;
    goto L0; 
  }
  // 0 <= r
  A[i++] = x; x = y;
  if ( 0 < r ) { 
    goto R0;
  }
  // r = 0
  // goto ML0;

 ML0: 
   /*
      |---)-----(----)-------(----|
     lo   i     lw  up       j    hi
      x -> ML 
   */
  if ( lw < i ) { i--;
    goto L1M;
  }
  // i <= lw
  y = A[lw];
  r = compareXY(y, p3);
  if ( r == 0 ) { lw--;
    goto ML0;
  }
  A[lw--] = x; x = y;
  if ( 0 < r ) {
    goto R0;
  }
  // r < 0
  goto L0;
  
 R0:
   /*
      |---)-----(----)-------(----|
     lo   i     lw  up       j    hi
      x -> R
   */
  if ( j < up ) { // A[j] = p3 !
    y = A[j]; A[j] = x; x = y;
    goto R1M;
  }
  // up <= j
  y = A[j]; 
  r = compareXY(y, p3);
  if ( 0 < r ) { j--;
    goto R0;
  }
  A[j--] = x; x = y;
  if ( r == 0 ) {
    goto MR0;
  }
  goto L0;
  
  MR0:
   /*
      |---)-----(----)-------(----|
     lo   i     lw  up       j    hi
      x -> MR
   */
  if ( j < up ) { j++;
    goto R1M;
  }
  // up <= j
  y = A[up];
  r = compareXY(y, p3);
  if ( r == 0 ) { up++;
    goto MR0;
  }
  A[up++] = x; x = y;
  if ( r < 0 ) {
    goto L0;
  }
  goto R0;

  L1L: 
   /*
      |---]---------)-------(----|
     lo   i        up       j    hi
      x -> L
   */
  if ( j < up ) { j++; A[lo] = x; 
    goto done; 
  }
  // up <= j
  y = A[up];
  r = compareXY(y, p3);
  if ( r == 0 ) { up++;
    goto L1L;
  }
  if ( r < 0 ) { A[up++] = A[++i]; A[i] = y;
    goto L1L;
  }
  // 0 < r
  A[up++] = A[++i]; A[i] = x; x = y;
  // goto L1R;

  L1R: 
   /*
      |---]---------)-------(----|
     lo   i        up       j    hi
      x -> R
   */
  if ( j < up ) { 
    A[lo] = A[i]; A[i--] = A[j]; A[j] = x;
    goto done; 
  }
  // up <= j
  y = A[j];
  r = compareXY(y, p3);
  if ( 0 < r ) { j--;
    goto L1R;
  }
  A[j--] = x; x = y;
  if ( r == 0 ) {
    goto L1M;
  }
  // r < 0
  goto L1L;

 L1M: 
   /*
      |---]---------)-------(----|
     lo   i        up       j    hi
      x -> M
   */
  if ( j < up ) { j++; A[lo] = A[i]; A[i--] = x;
    goto done;
  }
  // up <= j
  y = A[up];
  r = compareXY(y, p3);
  if ( r == 0 ) { up++;
    goto L1M;
  }
  A[up++] = x; x = y;
  if ( r < 0 ) {
    goto L1L;
  }
  goto L1R;

 R1R: 
   /*
      |---)---(-------------[----|
     lo   i   lw            j    hi
      x -> R
   */
  if ( lw < i ) {
    if ( i == lo2 ) { i = lo-1; // left empty
      A[lo] = A[--j]; A[j] = x;
      goto done;      
    }
    // lo2 < i
    A[lo] = A[--i]; A[i--] = A[--j]; A[j] = x;
    goto done;
  }
  // i <= lw
  y = A[lw];
  r = compareXY(y, p3);
  if ( r == 0 ) { lw--;
    goto R1R;
  }
  A[lw--] = A[--j]; A[j] = x; x = y;
  if ( r < 0 ) {
    goto R1L;
  }
  goto R1R;

 R1L: 
   /*
      |---)---(-------------[----|
     lo   i   lw            j    hi
      x -> L
   */
  if ( lw < i ) {
    A[lo] = x; i--;
    goto done;
  }
  // i <= lw
  y = A[i];
  r = compareXY(y, p3);
  if ( r < 0 ) { i++;
    goto R1L;
  }
  A[i++] = x; x = y;
  if ( r == 0 ) {
    goto R1M;
  }
  goto R1R;

 R1M: 
   /*
      |---)---(-------------[----|
     lo   i   lw            j    hi
      x -> M
   */
  if ( lw < i ) {
    if ( i == lo2 ) { i = lo-1; // left empty
      A[lo] = x;
       goto done;      
    }
    // lo2 < i
    A[lo] = A[--i]; A[i--] = x;
    goto done;
  }
  // i <= lw
  y = A[lw];
  r = compareXY(y, p3);
  if ( r == 0 ) { lw--;
    goto R1M;
  }
  A[lw--] = x; x = y;
  if ( r < 0 ) {
    goto R1L;
  }
  goto R1R;

 done: 
    /*
      |---]---------[---------|
     lo   i         j         hi
    */
    /*
      for ( z = lo; z <= i; z++ )
	// if ( p3 <= A[z] ) {
	if ( compareXY(p3, A[z]) <= 0 ) {
	  printf("doneL z %i\n", z);
	  printf("lo %i i %i lw %i up %i j %i hi %i\n", lo,i,lw,up,j,hi);
	  exit(0);
	}
      for ( z = i+1; z < j; z++ )
	// if ( p3 != A[z] ) {
	if ( compareXY(p3, A[z]) != 0 ) {
	  printf("doneM z %i\n", z);
	  printf("lo %i i %i lw %i up %i j %i hi %i\n", lo,i,lw,up,j,hi);
	  exit(0);
	}
      for ( z = j; z <= hi ; z++ )
	// if ( A[z] <= p3 ) {
	if ( compareXY(A[z], p3) <= 0 ) {
	  printf("doneR z %i\n", z);
	  printf("lo %i i %i lw %i up %i j %i hi %i\n", lo,i,lw,up,j,hi);
	  exit(0);
	}
      */
    if ( i - lo  < hi - j ) {
      if ( lo < i ) (*cut)(A, lo, i, depthLimit, compareXY);
      if ( j < hi ) (*cut)(A, j, hi, depthLimit, compareXY);
      return;
    }
    if ( j < hi ) (*cut)(A, j, hi, depthLimit, compareXY);
    if ( lo < i ) (*cut)(A, lo, i, depthLimit, compareXY);
} // end dflgm
