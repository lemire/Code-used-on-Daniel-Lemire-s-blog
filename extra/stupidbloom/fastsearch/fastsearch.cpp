
#include <vector>
#include <iostream>
#include <cmath>

// should be rich enough for most hashing purposes
struct parameters {
    uint64_t a;
    uint64_t b;
    int r;
};
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

double benchmark(std::function<uint64_t(uint64_t)> fingerprint, std::function<size_t(uint64_t, size_t)> location, size_t N = 2000000, size_t trial = 200000) {
    static std::vector<uint64_t> buffer;
    buffer.clear(); buffer.resize(N * 2 / 8); // two bytes per entry
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

uint64_t rotate(uint64_t x, unsigned int i) {
    if(i == 0) { return x; }
    return (x >> i) | (x << (64 - i));
}

double get_score(parameters p) {
    uint64_t a = p.a;
    uint64_t b = p.b;
    int r = p.r;
    std::function<size_t(uint64_t, size_t)> location = [](uint64_t x, size_t range) { return reduce(cheap_mix(x), range); };
    std::function<uint64_t(uint64_t)> fingerprint = [a,b,r](uint64_t x) { return x & ((x>>r)*a) & (x*b); };
    return benchmark(fingerprint, location);
}

double efficiency(double rate) {
    double bloom = - log(rate) / log(2);
    return 16.0 / bloom;
}

void demo() {
  splitmix_generator  gen;
  gen.state = 132321311111;
  double best = 1;
  parameters p{0x2a49edfa2a266dff , 0x32d43cdce50fed13, 44};
  best = get_score(p);
  std::cout << "best = "<< best << " ("<< efficiency(best) << ")" << std::endl;
  std::cout << "a = " << std::hex << p.a << " b = " << p.b << " r " << std::dec << p.r << std::endl;
  for(size_t i = 0; i < 10000; i++) {
      parameters tp{gen(), gen(), int(gen() & 63)};
      double rate = get_score(tp);
      if(rate < best) {
          best = rate;
          p = tp;
          std::cout << "best = "<< best << " ("<< efficiency(best) << ")" << std::endl;
          std::cout << "a = " << std::hex << p.a << " b = " << p.b << " r " << std::dec << p.r << std::endl;
      }
  }
}
 
int main() {

    std::function<size_t(uint64_t, size_t)> location = [](uint64_t x, size_t range) { return reduce(cheap_mix(x), range); };
    std::function<uint64_t(uint64_t)> fingerprint1 = [](uint64_t x) { 
        return uint64_t(1) << (x&63);
    };
    std::function<uint64_t(uint64_t)> fingerprint2 = [](uint64_t x) { 
        return (uint64_t(1) << (x&63)) | (uint64_t(1) << ((x/64)&63));
    };
    std::function<uint64_t(uint64_t)> fingerprint3 = [](uint64_t x) { 
        return (uint64_t(1) << (x&63)) | (uint64_t(1) << ((x/64)&63)) | (uint64_t(1) << ((x/4096)&63));
    };
    std::function<uint64_t(uint64_t)> fingerprint4 = [](uint64_t x) { 
        return (uint64_t(1) << (x&63)) | (uint64_t(1) << ((x/64)&63)) | (uint64_t(1) << ((x/4096)&63)) | (uint64_t(1) << ((x/262144)&63));
    };
    std::function<uint64_t(uint64_t)> fingerprint5 = [](uint64_t x) { 
        return (uint64_t(1) << (x&63)) | (uint64_t(1) << ((x/64)&63)) | (uint64_t(1) << ((x/4096)&63)) | (uint64_t(1) << ((x/262144)&63))| (uint64_t(1) << ((x/16777216)&63));
    };
    std::function<uint64_t(uint64_t)> fingerprint6 = [](uint64_t x) { 
        return (uint64_t(1) << (x&63)) | (uint64_t(1) << ((x/64)&63)) | (uint64_t(1) << ((x/4096)&63)) | (uint64_t(1) << ((x/262144)&63))| (uint64_t(1) << ((x/16777216)&63))| (uint64_t(1) << ((x/1073741824)&63));
    };
    std::function<uint64_t(uint64_t)> fingerprint7 = [](uint64_t x) { 
        return (uint64_t(1) << (x&63)) | (uint64_t(1) << ((x/64)&63)) | (uint64_t(1) << ((x/4096)&63)) | (uint64_t(1) << ((x/262144)&63))| (uint64_t(1) << ((x/16777216)&63))| (uint64_t(1) << ((x/1073741824)&63))| (uint64_t(1) << ((x/68719476736)&63));
    };
    std::cout << benchmark(fingerprint1, location) << std::endl;
    std::cout << benchmark(fingerprint2, location) << std::endl;
    std::cout << benchmark(fingerprint3, location) << std::endl;
    std::cout << benchmark(fingerprint4, location) << std::endl;
    std::cout << benchmark(fingerprint5, location) << std::endl;
    std::cout << benchmark(fingerprint6, location) << std::endl;
    std::cout << benchmark(fingerprint7, location) << std::endl;

    std::cout << efficiency(benchmark(fingerprint6, location)) << std::endl;

//    return benchmark(fingerprint, location);

}
