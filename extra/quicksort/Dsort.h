#ifndef DSORT
#define DSORT
extern void dflgm(void **A, int lo, int hi, int pivotx,
           void (*cut)(void**, int, int, int,
                       int (*)(const void*, const void*)),
		  int depthLimit, int (*compareXY)(const void*, const void*));
#include "Dsort.c"
#endif
