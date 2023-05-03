#include <stdio.h>
#include <stdlib.h>

int go() { 
	int x = 0;
	x += *((int *)NULL); 
	return x;
}

int main() { return go(); }
