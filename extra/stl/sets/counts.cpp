/**
 g++ -O3 -o counts counts.cpp -std=c++11
*/

#include "benchmark.h"
#include <algorithm>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <x86intrin.h>

typedef std::vector<uint32_t> vector;

template <typename T>
class count_back_inserter {
public:
    size_t & count;
    typedef void value_type;
    typedef void difference_type;
    typedef void pointer;
    typedef void reference;
    typedef std::output_iterator_tag iterator_category;
    count_back_inserter(size_t & c) : count(c) {};
    void operator=(const T &){ }
    count_back_inserter &operator *(){ return *this; }
    count_back_inserter &operator++(){ count++;return *this; }

};
typedef count_back_inserter<uint32_t> inserter;



static void arrayunion(vector &v1, vector &v2, vector &v) {
  v.clear();
  std::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(),
                 std::back_inserter(v));
}

static void arrayintersection(vector &v1, vector &v2, vector &v) {
  v.clear();
  std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(),
                        std::back_inserter(v));
}

static size_t arrayunion_count(vector &v1, vector &v2) {
  vector v;
  std::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(),
                 std::back_inserter(v));
  return v.size();
}

static size_t arrayintersection_count(vector &v1, vector &v2) {
  vector v;
  std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(),
                        std::back_inserter(v));
  return v.size();
}




// Counts the number of elements that would be in the union
template <class InputIterator1, class InputIterator2>
size_t set_union_size(InputIterator1 first1, InputIterator1 last1,
                      InputIterator2 first2, InputIterator2 last2)
{
    size_t __result = 0;
    for (; first1 != last1;)
    {
        if (first2 == last2)
            return __result + std::distance(first1, last1);
        if (*first2 < *first1)
        {
            ++__result;
            ++first2;
        }
        else
        {
            ++__result;
            if (!(*first1 < *first2))
                ++first2;
            ++first1;
        }
    }
    return __result + std::distance(first2, last2);
}



static size_t arrayunion_inserter_count(vector &v1, vector &v2) {
  size_t answer = 0;
  std::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(),
                 inserter(answer));
  return answer;
}

static size_t arrayintersection_inserter_count(vector &v1, vector &v2) {
  size_t answer = 0;
  std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(),
                 inserter(answer));
  return answer;
}

void demo(const int N) {
  printf("%d\n", N);
  vector v1;
  vector v2;
  for (int z = 0; z < N; ++z) {
    v1.push_back(3 * z + 1);
    v2.push_back(N - z);
  }
  std::sort(v1.begin(), v1.end());
  std::sort(v2.begin(), v2.end());

  size_t unionexpected = arrayunion_count(v1,v2);
  size_t interexpected = arrayintersection_count(v1,v2);
  const int repeat = 5;
  BEST_TIME(arrayunion_count(v1,v2),unionexpected, , repeat, 2 * N, true);
  BEST_TIME(arrayunion_inserter_count(v1,v2),unionexpected, , repeat, 2 * N, true);
  BEST_TIME(set_union_size(v1.begin(),v1.end(),v2.begin(),v2.end()),unionexpected, , repeat, 2 * N, true);

  BEST_TIME(arrayintersection_count(v1,v2),interexpected, , repeat, 2 * N, true);
  BEST_TIME(arrayintersection_inserter_count(v1,v2),interexpected, , repeat, 2 * N, true);
}

int main() {
  const int N = 1000000;
  demo(10);
//  demo(1000000);
//  demo(10 * 1000000);
}
