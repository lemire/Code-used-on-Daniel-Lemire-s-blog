#include "benchmark.h"
#include "pcg.h"

#include <stdexcept>

template <typename T>
void reservoirsampling(T *output, uint32_t howmany, const T *source,
                       uint32_t sourcelength) {
  if (howmany == 0)
    return;
  if (howmany > sourcelength) {
    throw ::std::logic_error(
        "I cannot sample the requested number. ");
  }
  size_t i;
  for (i = 0; i < howmany; i++) {
    output[i] = source[i];
  }
  for (; i < sourcelength; i++) {
    size_t nextpos = random_bounded(i);
    if (nextpos < howmany) {
      output[nextpos] = source[i];
    }
  }
}

template <typename T>
void shuffle(T *storage, uint32_t size) {
  uint32_t i;
  for (i = size; i > 1; i--) {
    uint32_t nextpos = random_bounded(i);
    uint32_t tmp = storage[i - 1];   // likely in cache
    uint32_t val = storage[nextpos]; // could be costly
    storage[i - 1] = val;
    storage[nextpos] = tmp; // you might have to read this store later
  }
}

// we are given two sources, source1 and source2,
// we want an output where we have howmanyfrom1 from source1
// and howmanyfrom2 from source2, all mixed up. Written to output.
template <typename T>
void mixme(const T *source1, uint32_t length1, const T *source2, uint32_t length2,
           uint32_t howmanyfrom1, uint32_t howmanyfrom2, T *output) {
  reservoirsampling(output, howmanyfrom1, source1, length1);
  reservoirsampling(output + howmanyfrom1, howmanyfrom2, source2, length2);
  shuffle(output, howmanyfrom1 + howmanyfrom2);
}

void demo() {
  uint32_t length1 = 1000000;
  uint32_t length2 = 1000000;
  uint32_t * source1 = new uint32_t[length1];
  uint32_t * source2 = new uint32_t[length2];
  for(uint32_t i = 0; i < length1; i++) {
    source1[i] = i;
  }
  for(uint32_t i = 0; i < length2; i++) {
    source2[i] = i + length1;
  }
  uint32_t howmanyfrom1 = length1 / 2;
  uint32_t howmanyfrom2 = length2 / 2;

  uint32_t * output = new uint32_t[howmanyfrom1 + howmanyfrom2];
  uint32_t size = length1 + length2;
  bool verbose = true;
  int repeat = 50;
   BEST_TIME_NOCHECK(mixme(source1,length1, source2, length2, howmanyfrom1, howmanyfrom2, output), , repeat, size, verbose) ;
  delete[] output;
  delete[] source1;
  delete[] source2;

}

int main() {
  demo();
  return EXIT_SUCCESS;
}
