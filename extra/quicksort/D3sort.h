#ifndef D3SORT
#define D3SORT
extern void dflgm3(void **A, int lo, int hi, 
	    int depthLimit, int (*compareXY)(const void*, const void*));
#include "D3sort.c"
#endif
