#include <stdint.h>

int sum_array(int* data, int count) {
    int sum = 0;
    for(int i = 0; i < count; i++) {
        sum += data[i];
    }
    return sum;
}

int sum_array_transformed(int* data, int count, int (*f)(int)) {
    int sum = 0;
    for(int i = 0; i < count; i++) {
        sum += f(data[i]);
    }
    return sum;
}