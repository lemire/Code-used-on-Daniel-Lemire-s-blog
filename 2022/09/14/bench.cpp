#include <benchmark/benchmark.h>
#include <random>
#include <vector>
#include "escape.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<uint32_t> dp(0, 100);

template< size_t SIZE, size_t NUM, size_t PROB, typename Fn >
static void StringReplace(benchmark::State& state) {
  // Create a big string (to optimize cache) 
  //  and pepper it with "bad" chars
  std::vector<char> strings(SIZE*NUM);
  std::memset( strings.data(), 'a', strings.size() );
  for ( size_t j=0; j<SIZE*NUM; ++j ) {
    if ( dp(gen) <= PROB ) strings[j] = '"';
  }
  std::vector<char*> input(NUM);
  for ( size_t j=0; j<NUM; ++j ) {
    input[j] = &strings[j*SIZE];
    input[j][SIZE-1] = '\0';
  }
  std::vector<char> out(2*SIZE);
  char* outs = out.data();
  Fn fn;
  for (auto _ : state) {
    for ( char* s : input ) {
      size_t res = fn.escape( s, SIZE, outs );
      benchmark::DoNotOptimize(res);
    }
    benchmark::DoNotOptimize(outs);
  }
}

#define TOTALSZ 10000000
#define BATCHTEST(sz) \
    BENCHMARK(StringReplace<sz,TOTALSZ/sz,10,Original>);  \
    BENCHMARK(StringReplace<sz,TOTALSZ/sz,10,Optimized>); \
    BENCHMARK(StringReplace<sz,TOTALSZ/sz,50,Original>);  \
    BENCHMARK(StringReplace<sz,TOTALSZ/sz,50,Optimized>);

BATCHTEST(2)
BATCHTEST(7)
BATCHTEST(8)
BATCHTEST(15)
BATCHTEST(27)
BATCHTEST(35)
BATCHTEST(125)
BATCHTEST(525)
BATCHTEST(1525)
BATCHTEST(5225)

BENCHMARK_MAIN();

