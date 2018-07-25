#include <stdbool.h>

// example by Travis Downs
int add(int* a, int* b) {
    int t = 0;
    bool quit = false;
    while (!quit) {
        quit = a[t];
        t += b[t] + 1;
    }
    return t;
}
