#include <stdio.h>
#include <stdlib.h>

void go() { 
	int x = 0;
	x += *((int *)NULL); 
}

int main() { go(); }
