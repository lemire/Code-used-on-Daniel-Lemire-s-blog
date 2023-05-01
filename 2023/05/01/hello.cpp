#include <stdio.h>
#include <stdlib.h>

void go() { printf("%d\n", *((int *)NULL)); }

int main() { go(); }
