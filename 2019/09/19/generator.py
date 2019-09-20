
print("""

#include <algorithm>
#include <climits>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <chrono>

#ifdef __clang__
#warning "LLVM is not recommended (does not generate cmov)"
#else
#ifndef __GNUG__
#warning "GNU G++ is recommended but not detected"
#endif
#endif



inline uint64_t splitmix64_stateless(uint64_t index) {
  uint64_t z = (index + UINT64_C(0x9E3779B97F4A7C15));
  z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
  z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
  return z ^ (z >> 31);
}

__attribute__((noinline)) size_t
element_access(int **__restrict data, size_t howmany, size_t arraysize,
               int *__restrict counter) {
  size_t hw = 0;
  for (size_t i = 0; i < howmany; i++) {
    const int *__restrict z = data[i];
    for (size_t c = 0; c < arraysize; c += 1) {
      *counter += z[c];
      hw++;
    }
  }
  return hw;
}

size_t branchy_search(const int *source, size_t n, int target) {
  size_t lo = 0;
  size_t hi = n;
  while (lo < hi) {
    size_t m = (lo + hi) / 2;
    if (target < source[m]) {
      hi = m;
    } else if (target > source[m]) {
      lo = m + 1;
    } else {
      return m;
    }
  }
  return hi;
}

__attribute__((noinline)) void
branchy(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
  for (size_t i = 0; i < howmany; i++) {
    solutions[i] = branchy_search(data[i], arraysize, targets[i]);
  }
}

size_t branchfree_search(const int *source, size_t n, int target) {
  if (n == 0)
    return 0;
  const int *base = source;
  while (n > 1) {
    size_t half = n >> 1;

    base = (base[half] < target) ? &base[half] : base;
    n -= half;
  }
  return (*base < target) + base - source;
}
__attribute__((noinline)) void
branchless_linked(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
  solutions[0] = branchfree_search(data[0], arraysize, targets[0]);
  for (size_t i = 1; i < howmany; i++) {
    solutions[i] = branchfree_search(data[i], arraysize, (targets[i] ^ solutions[i - 1]) % arraysize);
  }
}
__attribute__((noinline)) void
branchless(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {
  for (size_t i = 0; i < howmany; i++) {
    solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
  }
}
""")

def generatesearch(width):
  answer = ""
  header = "void binsearch{width}(int **__restrict data, size_t howmany, size_t arraysize, const int* targets, size_t * __restrict solutions) {{".format(width=width)
  answer += header
  fastheader= """
    if(arraysize == 0) return; // pathological
    size_t i = 0;
    if(howmany >= {width}) {{
      for(; i <= howmany - {width}; i += {width}) {{
""".format(width=width)
  answer += fastheader
  for j in range(width):
    answer+="        const int * base{j} = data[i + {j}];\n".format(j=j)
  answer += """        size_t n = arraysize;
        while (n > 1) {
          size_t half = n >> 1;
"""
  for j in range(width):
    answer+="          base{j} = (base{j}[half] < targets[i + {j}]) ? &base{j}[half] : base{j};\n".format(j=j)
  answer+= """          n -= half;
        }
"""
  for j in range(width):
    answer+="        solutions[i + {j}] = (base{j}[0] < targets[i + {j}]) + base{j} - data[i + {j}];\n".format(j=j)
  finisher = """      }
    }
    for (; i < howmany; i++) {
        solutions[i] = branchfree_search(data[i], arraysize, targets[i]);
    }
}"""
  answer += finisher
  return answer

MAX=64
for i in range(2,MAX+1):
  print(generatesearch(i))

print("""
int main() {
  size_t small = 1024;
  size_t large = 64 * 1000 * 1000 / small;

  printf("Initializing random data.\\n");
  std::vector<int> datavec(small * large); // important: memory is consecutive
  std::vector<int> targets;
  for (size_t i = 0; i < small; i++) {
    for (size_t z = 0; z < large; z++) {
      datavec[i * large + z] = splitmix64_stateless(i * large + z);
    }
    targets.push_back(datavec[i * large]);
    std::sort(&datavec[i * large], &datavec[i * large] + large);
    if((i % (small / 10)) == 0) printf(".");
    fflush(NULL);
  }
  printf("\\n");
  std::vector<size_t> solution(small);
  int ** data = new int*[small];
  for (size_t i = 0; i < small; i++) {
      data[i] = datavec.data() + i * large;;
  }
  int counter = 0;

""")
print("  for(int z = 0; z < 5; z++) element_access(data, small, large, & counter);")
print("  auto startref = std::chrono::high_resolution_clock::now();".format(width=i))
print("  branchless_linked(data, small, large, targets.data(), solution.data());")
print("  auto finishref = std::chrono::high_resolution_clock::now();".format(width=i))
print("  std::cout <<  \" ref : \" << std::chrono::duration_cast<std::chrono::nanoseconds>(finishref-startref).count() << std::endl;")


for i in range(2,MAX+1):
    print("  for(int z = 0; z < 5; z++) element_access(data, small, large, & counter);")
    print("  auto start{width} = std::chrono::high_resolution_clock::now();".format(width=i))
    print("  binsearch{width}(data, small, large, targets.data(), solution.data());".format(width=i))
    print("  auto finish{width} = std::chrono::high_resolution_clock::now();".format(width=i))
    print("  std::cout <<  \" {width} : \" << std::chrono::duration_cast<std::chrono::nanoseconds>(finish{width}-start{width}).count() << std::endl;".format(width=i))
    print("  std::cout <<  \" gain \" << std::chrono::duration_cast<std::chrono::nanoseconds>(finishref-startref).count() * 1.0 / std::chrono::duration_cast<std::chrono::nanoseconds>(finish{width}-start{width}).count() << std::endl;".format(width=i))

print("""
  delete[] data;
  if(counter == 0) printf("counter is zero\\n");
  return EXIT_SUCCESS;
}""")