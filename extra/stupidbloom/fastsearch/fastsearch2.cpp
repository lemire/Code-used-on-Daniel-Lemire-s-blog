
#include <vector>
#include <iostream>
#include <cmath>

// should be rich enough for most hashing purposes
struct parameters {
    uint64_t a;
    uint64_t b;
    uint64_t c;

    int r;
    int s;

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

double benchmark(std::function<uint64_t(uint64_t)> fingerprint1, std::function<uint64_t(uint64_t)> fingerprint2, std::function<size_t(uint64_t, size_t)> location, size_t N = 2000000, size_t trial = 200000) {
    static std::vector<uint64_t> buffer;
    buffer.clear(); buffer.resize(N * 2 / 8); // two bytes per entry
    splitmix_generator gen;
    for(size_t i = 0; i < N ; i++) {
        uint64_t key = gen();
        buffer[location(key,buffer.size())] |= fingerprint1(key);
        buffer[location(key,buffer.size())+1] |= fingerprint2(key);

    }
    size_t match{0};
    for(size_t i = 0; i < trial ; i++) {
        uint64_t key = gen();
        uint64_t print1 = fingerprint1(key);
        uint64_t print2 = fingerprint2(key);
        if(((buffer[location(key,buffer.size())] & print1) == print1)  && ((buffer[location(key,buffer.size())+1] & print2) == print2)){ match++; }
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
    uint64_t c = p.c;
    int r = p.r;
    int s = p.s;
    std::function<size_t(uint64_t, size_t)> location = [](uint64_t x, size_t range) { return reduce(cheap_mix(x), range-1); };
    std::function<uint64_t(uint64_t)> fingerprint1 = [a,b,r](uint64_t x) { return x & ((x>>r)*a) & (x*b) ; };
    std::function<uint64_t(uint64_t)> fingerprint2 = [a,c,s](uint64_t x) { return x & (x*a) & ((x>>s)*c) ; };

    return benchmark(fingerprint1, fingerprint2,location);
}

double efficiency(double rate) {
    double bloom = - log(rate) / log(2);
    return 16. / bloom;
}
 
int main() {
  splitmix_generator  gen;
  gen.state = 132321311111;
  double best = 1;
  parameters p{0x2a49edfa2a266dff , 0x32d43cdce50fed13, 0x32d43cdce50fed13, 44, 13};
  best = get_score(p);
  std::cout << "best = "<< best << " ("<< efficiency(best) << ")" << std::endl;
  std::cout << "a = " << std::hex << p.a << " b = " << p.b << " b = " << p.c << " r " << std::dec << p.r<< " s " << std::dec << p.s << std::endl;
  for(size_t i = 0; i < 10000; i++) {
      parameters tp{gen(), gen(),gen(), int(gen() & 63), int(gen() & 63)};
      double rate = get_score(tp);
      if(rate < best) {
          best = rate;
          p = tp;
          std::cout << "best = "<< best << " ("<< efficiency(best) << ")" << std::endl;
          std::cout << "a = " << std::hex << p.a << " b = " << p.b << " b = " << p.c << " r " << std::dec << p.r<< " s " << std::dec << p.s << std::endl;
      }
  }
}
