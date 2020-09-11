
#include <algorithm>
#include <chrono>
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <charconv>
#include <random>

#include <locale.h>

/**
 * Determining whether we should import xlocale.h or not is 
 * a bit of a nightmare.
 */
#ifdef __GLIBC__
#include <features.h>
#if !((__GLIBC__ > 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ > 25)))
#include <xlocale.h> // old glibc
#endif
#else // not glibc
#ifndef _MSC_VER // assume that everything that is not GLIBC and not Visual Studio needs xlocale.h
#include <xlocale.h>
#endif
#endif

double findmax_strtod(std::vector<std::string> &s) {
  double answer = 0;
  double x = 0;
  for (std::string &st : s) {
    char *pr = (char *)st.data();
#ifdef _WIN32
    static _locale_t c_locale = _create_locale(LC_ALL, "C");
    x = _strtod_l(st.data(), &pr,  c_locale);
#else
    static locale_t c_locale = newlocale(LC_ALL_MASK, "C", NULL);
    x = strtod_l(st.data(), &pr,  c_locale);
#endif
    if (pr == st.data()) {
      throw std::runtime_error("bug in findmax_strtod");
    }
    answer = answer > x ? answer : x;
  }
  return answer;
}
#if defined(_MSC_VER)
#define FROM_CHARS_AVAILABLE_MAYBE
#endif

#ifdef FROM_CHARS_AVAILABLE_MAYBE
double findmax_from_chars(std::vector<std::string> &s) {
  double answer = 0;
  double x = 0;
  for (std::string &st : s) {
    auto [p, ec] = std::from_chars(st.data(), st.data() + st.size(), x);
    if (p == st.data()) {
      throw std::runtime_error("bug in findmax_from_chars");
    }
    answer = answer > x ? answer : x;
  }
  return answer;
}
#endif

template <class T>
std::pair<double, double> time_it_ns(std::vector<std::string> &lines,
                                     T const &function, size_t repeat) {
  std::chrono::high_resolution_clock::time_point t1, t2;
  double average = 0;
  double min_value = DBL_MAX;
  for (size_t i = 0; i < repeat; i++) {
    t1 = std::chrono::high_resolution_clock::now();
    double ts = function(lines);
    if (ts == 0) {
      printf("bug\n");
    }
    t2 = std::chrono::high_resolution_clock::now();
    double dif =
        std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    average += dif;
    min_value = min_value < dif ? min_value : dif;
  }
  return std::make_pair(min_value, average);
}

void process(std::vector<std::string> &lines, size_t volume) {
  size_t repeat = 40;
  double volumeMB = volume / (1024. * 1024.);
  std::cout << "volume = " << volumeMB << " MB "<< std::endl;
  auto pretty_print = [volumeMB](std::string name,
                                 std::pair<double, double> result) {
    printf("%-40s: %8.2f MB/s (+/- %.1f %%)\n", name.data(),
           volumeMB * 1000000000 / result.first,
           100 - (result.second - result.first) * 100.0 / result.second);
  };
  pretty_print("strtod", time_it_ns(lines, findmax_strtod, repeat));
#ifdef FROM_CHARS_AVAILABLE_MAYBE
  pretty_print("from_chars", time_it_ns(lines, findmax_from_chars, repeat));
#endif 
}


/**
 * This will generate a string with exactly the number of digits
 * that are required to always be able to recover the original
 * number (irrespective of the number). So 17 digits in the case
 * of a double.
 * E.g., 3.7018502067730191e-02
 */
template <typename T> std::string accurate_to_string(T d) {
  std::string answer;
  answer.resize(64);
  auto written = std::snprintf(answer.data(), 64, "%.*e",
                              std::numeric_limits<T>::max_digits10 - 1, d);
  answer.resize(written);
  return answer;
}

void demo(size_t howmany) {
  std::cout << "# parsing random integers in the range [0,1)" << std::endl;
  std::vector<std::string> lines;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dis(0, 1);
  lines.reserve(howmany); // let us reserve plenty of memory.
  size_t volume = 0;
  for (size_t i = 0; i < howmany; i++) {
    double x =  dis(gen);
    std::string line = accurate_to_string(x);
    volume += line.size();
    lines.push_back(line);
  }
  process(lines, volume);
}

int main(int argc, char **argv) {
    demo(100 * 1000);
}
