/* file 1 -- adding test */
#include <stdlib.h>
int main(int argc, char *argv[]) {
    int N = atoi(argv[1]);
    long int k=0;
    int sum=0;
    for(int j = 0; j<1000;++j) for(k=0; k < N; ++k) sum+= k;
    return 0;
}
 