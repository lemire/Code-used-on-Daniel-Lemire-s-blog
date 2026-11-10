#ifndef KERNELS_H
#define KERNELS_H
#include <string>
#include <vector>

// Each sums the numbers, parsed with fast_float::from_chars and:
double sum_none(const std::vector<std::string> &numbers);  // no hint
double sum_sign(const std::vector<std::string> &numbers);  // unpredictable_sign
double sum_short(const std::vector<std::string> &numbers); // short_numbers
double
sum_nearest(const std::vector<std::string> &numbers); // assume_round_to_nearest
double
sum_sign_nearest(const std::vector<std::string> &numbers); // sign + rounding
double
sum_short_nearest(const std::vector<std::string> &numbers); // short + rounding
double sum_all(const std::vector<std::string> &numbers);    // all three

#endif
