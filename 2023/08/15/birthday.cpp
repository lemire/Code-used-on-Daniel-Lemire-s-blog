#include <cstdint>
#include <algorithm>
#include <vector>



/**
* D. H. Lehmer, Mathematical methods in large-scale computing units.
* Proceedings of a Second Symposium on Large Scale Digital Calculating
* Machinery;
* Annals of the Computation Laboratory, Harvard Univ. 26 (1951), pp. 141-146.
*
* P L'Ecuyer,  Tables of linear congruential generators of different sizes and
* good lattice structure. Mathematics of Computation of the American
* Mathematical
* Society 68.225 (1999): 249-260.
*/
static inline uint64_t lehmer64(__uint128_t &g_lehmer64_state) {
  g_lehmer64_state *= UINT64_C(0xda942042e4dd58b5);
  return g_lehmer64_state >> 64;
}

bool has_collision(uint64_t* array, size_t length) {
    std::sort(array, array + length);
    for (size_t i = 1; i < length; ++i) {
        if (array[i - 1] == array[i]) {
            return true;
        }
    }
    return false;
}

int main() {
    constexpr size_t array_size = 500000000;
    
    constexpr size_t trials = 1000;
    size_t collisions = 0;
    #pragma omp parallel for reduction(+:collisions)
    for(size_t i = 0; i < trials; i++) {
        __uint128_t g_lehmer64_state = 123456789 + i;

        std::vector<uint64_t> array(array_size);
        for(size_t k = 0; k< array_size; k++) {
            array[k] = lehmer64(g_lehmer64_state);
        }
        if (has_collision(array.data(), array_size)) {
            collisions++;
            printf("!");
        }
        printf(".");
        fflush(stdout);
    }
    printf("%f\n", (double)collisions / trials);
    return EXIT_SUCCESS;
}