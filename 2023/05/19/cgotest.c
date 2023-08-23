
#include "cgotest.h"
#include <stdlib.h>
int addition(int x, int y) {
    return x + y;
}

void allocate_free() {
    char* x = (char*)malloc(100);
    if(x == NULL) {
        return;
    }
    free(x);
}

char* allocate() {
    return (char*)malloc(100);
}

void free_allocated(char *c) {
    free(c);
}