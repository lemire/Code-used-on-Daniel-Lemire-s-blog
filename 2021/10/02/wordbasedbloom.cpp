
#include <vector>
#include <iostream>
#include <cmath>


struct splitmix_generator {
  uint64_t state;

  inline uint64_t operator()() {
    uint64_t z = (state += UINT64_C(0x9E3779B97F4A7C15));
    z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
    return z ^ (z >> 31);
  }
};

uint64_t cheap_mix(uint64_t key) {
  return (key ^ (key >> 31)) * UINT64_C(0x85D059AA333121CF); 
}

uint64_t reduce(uint64_t key, uint64_t range) {
    return ((__uint128_t) key * range) >> 64;
}

double benchmark(double bits_per_entry, std::function<uint64_t(uint64_t)> fingerprint, std::function<size_t(uint64_t, size_t)> location, size_t N = 2000000, size_t trial = 200000) {
    static std::vector<uint64_t> buffer;
    buffer.clear(); 
    buffer.resize(size_t(round(N * (bits_per_entry / 8) / sizeof(uint64_t)))); // two bytes per entry
    splitmix_generator gen;
    for(size_t i = 0; i < N ; i++) {
        uint64_t key = gen();
        buffer[location(key,buffer.size())] |= fingerprint(key);
    }
    size_t match{0};
    for(size_t i = 0; i < trial ; i++) {
        uint64_t key = gen();
        uint64_t print = fingerprint(key);
        if((buffer[location(key,buffer.size())] & print) == print) { match++; }
    }
    return double(match)/double(trial);
}


double efficiency(double bits_per_entry, double rate) {
    double ideal = - log(rate) / log(2);
    return bits_per_entry / ideal;
}

 
int main() {

    std::function<size_t(uint64_t, size_t)> location = [](uint64_t x, size_t range) { return reduce(cheap_mix(x), range); };

    std::function<uint64_t(uint64_t)> fingerprint5 = [](uint64_t x) { 
        return (uint64_t(1) << (x&63)) 
            | (uint64_t(1) << ((x>>6)&63)) 
            | (uint64_t(1) << ((x>>12)&63)) 
            | (uint64_t(1) << ((x>>18)&63))
            | (uint64_t(1) << ((x>>24)&63));
    };
    double bits_per_entry = 12;
    std::cout << benchmark(bits_per_entry, fingerprint5, location) << " : "<< efficiency(bits_per_entry, benchmark(bits_per_entry, fingerprint5, location)) << std::endl;
}
