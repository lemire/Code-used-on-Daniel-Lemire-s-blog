#include <iostream>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
static uint64_t murmur64(uint64_t h) {
  h ^= h >> 33;
  h *= UINT64_C(0xff51afd7ed558ccd);
  h ^= h >> 33;
  h *= UINT64_C(0xc4ceb9fe1a85ec53);
  h ^= h >> 33;
  return h;
}

template <size_t howmanylanes>
uint64_t access(uint64_t *bigarray, size_t length, size_t howmanyhits) {
  size_t howmanyhits_perlane = howmanyhits / howmanylanes;
  uint64_t lanesums[howmanylanes];
  for (size_t i = 0; i < howmanylanes; i++)
    lanesums[i] = 0;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    for (size_t i = 0; i < howmanylanes; i++) {
      size_t laneindexes = murmur64(lanesums[i] + i) & (length - 1);
      lanesums[i] += bigarray[laneindexes];
    }
  }
  uint64_t sum = 0;
  for (size_t i = 0; i < howmanylanes; i++)
    sum += lanesums[i];
  // std::cout << howmanylanes << " : " << sum << std::endl;
  return sum;
}
uint64_t naked_access_1(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  size_t howmanyhits_perlane = howmanyhits / 1;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
  }
  return val1;
}

float time_me1(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_1(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 1 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_2(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  size_t howmanyhits_perlane = howmanyhits / 2;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
  }
  return val1 + val2;
}

float time_me2(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_2(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 2 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_3(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  size_t howmanyhits_perlane = howmanyhits / 3;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
  }
  return val1 + val2 + val3;
}

float time_me3(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_3(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 3 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_4(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  size_t howmanyhits_perlane = howmanyhits / 4;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
  }
  return val1 + val2 + val3 + val4;
}

float time_me4(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_4(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 4 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_5(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  size_t howmanyhits_perlane = howmanyhits / 5;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
  }
  return val1 + val2 + val3 + val4 + val5;
}

float time_me5(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_5(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 5 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_6(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  size_t howmanyhits_perlane = howmanyhits / 6;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
  }
  return val1 + val2 + val3 + val4 + val5 + val6;
}

float time_me6(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_6(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 6 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_7(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  size_t howmanyhits_perlane = howmanyhits / 7;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7;
}

float time_me7(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_7(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 7 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_8(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  size_t howmanyhits_perlane = howmanyhits / 8;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8;
}

float time_me8(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_8(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 8 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_9(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  size_t howmanyhits_perlane = howmanyhits / 9;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9;
}

float time_me9(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_9(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 9 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_10(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  uint64_t val10 = 10;
  size_t howmanyhits_perlane = howmanyhits / 10;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
    val10 = bigarray[val10];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10;
}

float time_me10(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_10(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 10 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_11(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  uint64_t val10 = 10;
  uint64_t val11 = 11;
  size_t howmanyhits_perlane = howmanyhits / 11;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
    val10 = bigarray[val10];
    val11 = bigarray[val11];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10 +
         val11;
}

float time_me11(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_11(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 11 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_12(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  uint64_t val10 = 10;
  uint64_t val11 = 11;
  uint64_t val12 = 12;
  size_t howmanyhits_perlane = howmanyhits / 12;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
    val10 = bigarray[val10];
    val11 = bigarray[val11];
    val12 = bigarray[val12];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10 +
         val11 + val12;
}

float time_me12(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_12(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 12 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_13(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  uint64_t val10 = 10;
  uint64_t val11 = 11;
  uint64_t val12 = 12;
  uint64_t val13 = 13;
  size_t howmanyhits_perlane = howmanyhits / 13;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
    val10 = bigarray[val10];
    val11 = bigarray[val11];
    val12 = bigarray[val12];
    val13 = bigarray[val13];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10 +
         val11 + val12 + val13;
}

float time_me13(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_13(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 13 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_14(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  uint64_t val10 = 10;
  uint64_t val11 = 11;
  uint64_t val12 = 12;
  uint64_t val13 = 13;
  uint64_t val14 = 14;
  size_t howmanyhits_perlane = howmanyhits / 14;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
    val10 = bigarray[val10];
    val11 = bigarray[val11];
    val12 = bigarray[val12];
    val13 = bigarray[val13];
    val14 = bigarray[val14];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10 +
         val11 + val12 + val13 + val14;
}

float time_me14(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_14(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 14 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_15(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  uint64_t val10 = 10;
  uint64_t val11 = 11;
  uint64_t val12 = 12;
  uint64_t val13 = 13;
  uint64_t val14 = 14;
  uint64_t val15 = 15;
  size_t howmanyhits_perlane = howmanyhits / 15;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
    val10 = bigarray[val10];
    val11 = bigarray[val11];
    val12 = bigarray[val12];
    val13 = bigarray[val13];
    val14 = bigarray[val14];
    val15 = bigarray[val15];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10 +
         val11 + val12 + val13 + val14 + val15;
}

float time_me15(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_15(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 15 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_16(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  uint64_t val10 = 10;
  uint64_t val11 = 11;
  uint64_t val12 = 12;
  uint64_t val13 = 13;
  uint64_t val14 = 14;
  uint64_t val15 = 15;
  uint64_t val16 = 16;
  size_t howmanyhits_perlane = howmanyhits / 16;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
    val10 = bigarray[val10];
    val11 = bigarray[val11];
    val12 = bigarray[val12];
    val13 = bigarray[val13];
    val14 = bigarray[val14];
    val15 = bigarray[val15];
    val16 = bigarray[val16];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10 +
         val11 + val12 + val13 + val14 + val15 + val16;
}

float time_me16(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_16(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 16 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_17(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  uint64_t val10 = 10;
  uint64_t val11 = 11;
  uint64_t val12 = 12;
  uint64_t val13 = 13;
  uint64_t val14 = 14;
  uint64_t val15 = 15;
  uint64_t val16 = 16;
  uint64_t val17 = 17;
  size_t howmanyhits_perlane = howmanyhits / 17;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
    val10 = bigarray[val10];
    val11 = bigarray[val11];
    val12 = bigarray[val12];
    val13 = bigarray[val13];
    val14 = bigarray[val14];
    val15 = bigarray[val15];
    val16 = bigarray[val16];
    val17 = bigarray[val17];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10 +
         val11 + val12 + val13 + val14 + val15 + val16 + val17;
}

float time_me17(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_17(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 17 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_18(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  uint64_t val10 = 10;
  uint64_t val11 = 11;
  uint64_t val12 = 12;
  uint64_t val13 = 13;
  uint64_t val14 = 14;
  uint64_t val15 = 15;
  uint64_t val16 = 16;
  uint64_t val17 = 17;
  uint64_t val18 = 18;
  size_t howmanyhits_perlane = howmanyhits / 18;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
    val10 = bigarray[val10];
    val11 = bigarray[val11];
    val12 = bigarray[val12];
    val13 = bigarray[val13];
    val14 = bigarray[val14];
    val15 = bigarray[val15];
    val16 = bigarray[val16];
    val17 = bigarray[val17];
    val18 = bigarray[val18];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10 +
         val11 + val12 + val13 + val14 + val15 + val16 + val17 + val18;
}

float time_me18(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_18(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 18 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_19(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  uint64_t val10 = 10;
  uint64_t val11 = 11;
  uint64_t val12 = 12;
  uint64_t val13 = 13;
  uint64_t val14 = 14;
  uint64_t val15 = 15;
  uint64_t val16 = 16;
  uint64_t val17 = 17;
  uint64_t val18 = 18;
  uint64_t val19 = 19;
  size_t howmanyhits_perlane = howmanyhits / 19;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
    val10 = bigarray[val10];
    val11 = bigarray[val11];
    val12 = bigarray[val12];
    val13 = bigarray[val13];
    val14 = bigarray[val14];
    val15 = bigarray[val15];
    val16 = bigarray[val16];
    val17 = bigarray[val17];
    val18 = bigarray[val18];
    val19 = bigarray[val19];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10 +
         val11 + val12 + val13 + val14 + val15 + val16 + val17 + val18 + val19;
}

float time_me19(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_19(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 19 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_20(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  uint64_t val10 = 10;
  uint64_t val11 = 11;
  uint64_t val12 = 12;
  uint64_t val13 = 13;
  uint64_t val14 = 14;
  uint64_t val15 = 15;
  uint64_t val16 = 16;
  uint64_t val17 = 17;
  uint64_t val18 = 18;
  uint64_t val19 = 19;
  uint64_t val20 = 20;
  size_t howmanyhits_perlane = howmanyhits / 20;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
    val10 = bigarray[val10];
    val11 = bigarray[val11];
    val12 = bigarray[val12];
    val13 = bigarray[val13];
    val14 = bigarray[val14];
    val15 = bigarray[val15];
    val16 = bigarray[val16];
    val17 = bigarray[val17];
    val18 = bigarray[val18];
    val19 = bigarray[val19];
    val20 = bigarray[val20];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10 +
         val11 + val12 + val13 + val14 + val15 + val16 + val17 + val18 + val19 +
         val20;
}

float time_me20(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_20(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 20 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_21(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  uint64_t val10 = 10;
  uint64_t val11 = 11;
  uint64_t val12 = 12;
  uint64_t val13 = 13;
  uint64_t val14 = 14;
  uint64_t val15 = 15;
  uint64_t val16 = 16;
  uint64_t val17 = 17;
  uint64_t val18 = 18;
  uint64_t val19 = 19;
  uint64_t val20 = 20;
  uint64_t val21 = 21;
  size_t howmanyhits_perlane = howmanyhits / 21;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
    val10 = bigarray[val10];
    val11 = bigarray[val11];
    val12 = bigarray[val12];
    val13 = bigarray[val13];
    val14 = bigarray[val14];
    val15 = bigarray[val15];
    val16 = bigarray[val16];
    val17 = bigarray[val17];
    val18 = bigarray[val18];
    val19 = bigarray[val19];
    val20 = bigarray[val20];
    val21 = bigarray[val21];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10 +
         val11 + val12 + val13 + val14 + val15 + val16 + val17 + val18 + val19 +
         val20 + val21;
}

float time_me21(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_21(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 21 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_22(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  uint64_t val10 = 10;
  uint64_t val11 = 11;
  uint64_t val12 = 12;
  uint64_t val13 = 13;
  uint64_t val14 = 14;
  uint64_t val15 = 15;
  uint64_t val16 = 16;
  uint64_t val17 = 17;
  uint64_t val18 = 18;
  uint64_t val19 = 19;
  uint64_t val20 = 20;
  uint64_t val21 = 21;
  uint64_t val22 = 22;
  size_t howmanyhits_perlane = howmanyhits / 22;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
    val10 = bigarray[val10];
    val11 = bigarray[val11];
    val12 = bigarray[val12];
    val13 = bigarray[val13];
    val14 = bigarray[val14];
    val15 = bigarray[val15];
    val16 = bigarray[val16];
    val17 = bigarray[val17];
    val18 = bigarray[val18];
    val19 = bigarray[val19];
    val20 = bigarray[val20];
    val21 = bigarray[val21];
    val22 = bigarray[val22];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10 +
         val11 + val12 + val13 + val14 + val15 + val16 + val17 + val18 + val19 +
         val20 + val21 + val22;
}

float time_me22(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_22(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 22 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_23(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  uint64_t val10 = 10;
  uint64_t val11 = 11;
  uint64_t val12 = 12;
  uint64_t val13 = 13;
  uint64_t val14 = 14;
  uint64_t val15 = 15;
  uint64_t val16 = 16;
  uint64_t val17 = 17;
  uint64_t val18 = 18;
  uint64_t val19 = 19;
  uint64_t val20 = 20;
  uint64_t val21 = 21;
  uint64_t val22 = 22;
  uint64_t val23 = 23;
  size_t howmanyhits_perlane = howmanyhits / 23;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
    val10 = bigarray[val10];
    val11 = bigarray[val11];
    val12 = bigarray[val12];
    val13 = bigarray[val13];
    val14 = bigarray[val14];
    val15 = bigarray[val15];
    val16 = bigarray[val16];
    val17 = bigarray[val17];
    val18 = bigarray[val18];
    val19 = bigarray[val19];
    val20 = bigarray[val20];
    val21 = bigarray[val21];
    val22 = bigarray[val22];
    val23 = bigarray[val23];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10 +
         val11 + val12 + val13 + val14 + val15 + val16 + val17 + val18 + val19 +
         val20 + val21 + val22 + val23;
}

float time_me23(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_23(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 23 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_24(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  uint64_t val10 = 10;
  uint64_t val11 = 11;
  uint64_t val12 = 12;
  uint64_t val13 = 13;
  uint64_t val14 = 14;
  uint64_t val15 = 15;
  uint64_t val16 = 16;
  uint64_t val17 = 17;
  uint64_t val18 = 18;
  uint64_t val19 = 19;
  uint64_t val20 = 20;
  uint64_t val21 = 21;
  uint64_t val22 = 22;
  uint64_t val23 = 23;
  uint64_t val24 = 24;
  size_t howmanyhits_perlane = howmanyhits / 24;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
    val10 = bigarray[val10];
    val11 = bigarray[val11];
    val12 = bigarray[val12];
    val13 = bigarray[val13];
    val14 = bigarray[val14];
    val15 = bigarray[val15];
    val16 = bigarray[val16];
    val17 = bigarray[val17];
    val18 = bigarray[val18];
    val19 = bigarray[val19];
    val20 = bigarray[val20];
    val21 = bigarray[val21];
    val22 = bigarray[val22];
    val23 = bigarray[val23];
    val24 = bigarray[val24];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10 +
         val11 + val12 + val13 + val14 + val15 + val16 + val17 + val18 + val19 +
         val20 + val21 + val22 + val23 + val24;
}

float time_me24(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_24(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 24 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_25(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  uint64_t val10 = 10;
  uint64_t val11 = 11;
  uint64_t val12 = 12;
  uint64_t val13 = 13;
  uint64_t val14 = 14;
  uint64_t val15 = 15;
  uint64_t val16 = 16;
  uint64_t val17 = 17;
  uint64_t val18 = 18;
  uint64_t val19 = 19;
  uint64_t val20 = 20;
  uint64_t val21 = 21;
  uint64_t val22 = 22;
  uint64_t val23 = 23;
  uint64_t val24 = 24;
  uint64_t val25 = 25;
  size_t howmanyhits_perlane = howmanyhits / 25;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
    val10 = bigarray[val10];
    val11 = bigarray[val11];
    val12 = bigarray[val12];
    val13 = bigarray[val13];
    val14 = bigarray[val14];
    val15 = bigarray[val15];
    val16 = bigarray[val16];
    val17 = bigarray[val17];
    val18 = bigarray[val18];
    val19 = bigarray[val19];
    val20 = bigarray[val20];
    val21 = bigarray[val21];
    val22 = bigarray[val22];
    val23 = bigarray[val23];
    val24 = bigarray[val24];
    val25 = bigarray[val25];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10 +
         val11 + val12 + val13 + val14 + val15 + val16 + val17 + val18 + val19 +
         val20 + val21 + val22 + val23 + val24 + val25;
}

float time_me25(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_25(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 25 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_26(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  uint64_t val10 = 10;
  uint64_t val11 = 11;
  uint64_t val12 = 12;
  uint64_t val13 = 13;
  uint64_t val14 = 14;
  uint64_t val15 = 15;
  uint64_t val16 = 16;
  uint64_t val17 = 17;
  uint64_t val18 = 18;
  uint64_t val19 = 19;
  uint64_t val20 = 20;
  uint64_t val21 = 21;
  uint64_t val22 = 22;
  uint64_t val23 = 23;
  uint64_t val24 = 24;
  uint64_t val25 = 25;
  uint64_t val26 = 26;
  size_t howmanyhits_perlane = howmanyhits / 26;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
    val10 = bigarray[val10];
    val11 = bigarray[val11];
    val12 = bigarray[val12];
    val13 = bigarray[val13];
    val14 = bigarray[val14];
    val15 = bigarray[val15];
    val16 = bigarray[val16];
    val17 = bigarray[val17];
    val18 = bigarray[val18];
    val19 = bigarray[val19];
    val20 = bigarray[val20];
    val21 = bigarray[val21];
    val22 = bigarray[val22];
    val23 = bigarray[val23];
    val24 = bigarray[val24];
    val25 = bigarray[val25];
    val26 = bigarray[val26];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10 +
         val11 + val12 + val13 + val14 + val15 + val16 + val17 + val18 + val19 +
         val20 + val21 + val22 + val23 + val24 + val25 + val26;
}

float time_me26(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_26(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 26 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_27(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  uint64_t val10 = 10;
  uint64_t val11 = 11;
  uint64_t val12 = 12;
  uint64_t val13 = 13;
  uint64_t val14 = 14;
  uint64_t val15 = 15;
  uint64_t val16 = 16;
  uint64_t val17 = 17;
  uint64_t val18 = 18;
  uint64_t val19 = 19;
  uint64_t val20 = 20;
  uint64_t val21 = 21;
  uint64_t val22 = 22;
  uint64_t val23 = 23;
  uint64_t val24 = 24;
  uint64_t val25 = 25;
  uint64_t val26 = 26;
  uint64_t val27 = 27;
  size_t howmanyhits_perlane = howmanyhits / 27;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
    val10 = bigarray[val10];
    val11 = bigarray[val11];
    val12 = bigarray[val12];
    val13 = bigarray[val13];
    val14 = bigarray[val14];
    val15 = bigarray[val15];
    val16 = bigarray[val16];
    val17 = bigarray[val17];
    val18 = bigarray[val18];
    val19 = bigarray[val19];
    val20 = bigarray[val20];
    val21 = bigarray[val21];
    val22 = bigarray[val22];
    val23 = bigarray[val23];
    val24 = bigarray[val24];
    val25 = bigarray[val25];
    val26 = bigarray[val26];
    val27 = bigarray[val27];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10 +
         val11 + val12 + val13 + val14 + val15 + val16 + val17 + val18 + val19 +
         val20 + val21 + val22 + val23 + val24 + val25 + val26 + val27;
}

float time_me27(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_27(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 27 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_28(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  uint64_t val10 = 10;
  uint64_t val11 = 11;
  uint64_t val12 = 12;
  uint64_t val13 = 13;
  uint64_t val14 = 14;
  uint64_t val15 = 15;
  uint64_t val16 = 16;
  uint64_t val17 = 17;
  uint64_t val18 = 18;
  uint64_t val19 = 19;
  uint64_t val20 = 20;
  uint64_t val21 = 21;
  uint64_t val22 = 22;
  uint64_t val23 = 23;
  uint64_t val24 = 24;
  uint64_t val25 = 25;
  uint64_t val26 = 26;
  uint64_t val27 = 27;
  uint64_t val28 = 28;
  size_t howmanyhits_perlane = howmanyhits / 28;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
    val10 = bigarray[val10];
    val11 = bigarray[val11];
    val12 = bigarray[val12];
    val13 = bigarray[val13];
    val14 = bigarray[val14];
    val15 = bigarray[val15];
    val16 = bigarray[val16];
    val17 = bigarray[val17];
    val18 = bigarray[val18];
    val19 = bigarray[val19];
    val20 = bigarray[val20];
    val21 = bigarray[val21];
    val22 = bigarray[val22];
    val23 = bigarray[val23];
    val24 = bigarray[val24];
    val25 = bigarray[val25];
    val26 = bigarray[val26];
    val27 = bigarray[val27];
    val28 = bigarray[val28];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10 +
         val11 + val12 + val13 + val14 + val15 + val16 + val17 + val18 + val19 +
         val20 + val21 + val22 + val23 + val24 + val25 + val26 + val27 + val28;
}

float time_me28(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_28(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 28 << " " << mintime << std::endl;
  return mintime;
}

uint64_t naked_access_29(uint64_t *bigarray, size_t howmanyhits) {
  uint64_t val1 = 1;
  uint64_t val2 = 2;
  uint64_t val3 = 3;
  uint64_t val4 = 4;
  uint64_t val5 = 5;
  uint64_t val6 = 6;
  uint64_t val7 = 7;
  uint64_t val8 = 8;
  uint64_t val9 = 9;
  uint64_t val10 = 10;
  uint64_t val11 = 11;
  uint64_t val12 = 12;
  uint64_t val13 = 13;
  uint64_t val14 = 14;
  uint64_t val15 = 15;
  uint64_t val16 = 16;
  uint64_t val17 = 17;
  uint64_t val18 = 18;
  uint64_t val19 = 19;
  uint64_t val20 = 20;
  uint64_t val21 = 21;
  uint64_t val22 = 22;
  uint64_t val23 = 23;
  uint64_t val24 = 24;
  uint64_t val25 = 25;
  uint64_t val26 = 26;
  uint64_t val27 = 27;
  uint64_t val28 = 28;
  uint64_t val29 = 29;
  size_t howmanyhits_perlane = howmanyhits / 29;
  for (size_t counter = 0; counter < howmanyhits_perlane; counter++) {
    val1 = bigarray[val1];
    val2 = bigarray[val2];
    val3 = bigarray[val3];
    val4 = bigarray[val4];
    val5 = bigarray[val5];
    val6 = bigarray[val6];
    val7 = bigarray[val7];
    val8 = bigarray[val8];
    val9 = bigarray[val9];
    val10 = bigarray[val10];
    val11 = bigarray[val11];
    val12 = bigarray[val12];
    val13 = bigarray[val13];
    val14 = bigarray[val14];
    val15 = bigarray[val15];
    val16 = bigarray[val16];
    val17 = bigarray[val17];
    val18 = bigarray[val18];
    val19 = bigarray[val19];
    val20 = bigarray[val20];
    val21 = bigarray[val21];
    val22 = bigarray[val22];
    val23 = bigarray[val23];
    val24 = bigarray[val24];
    val25 = bigarray[val25];
    val26 = bigarray[val26];
    val27 = bigarray[val27];
    val28 = bigarray[val28];
    val29 = bigarray[val29];
  }
  return val1 + val2 + val3 + val4 + val5 + val6 + val7 + val8 + val9 + val10 +
         val11 + val12 + val13 + val14 + val15 + val16 + val17 + val18 + val19 +
         val20 + val21 + val22 + val23 + val24 + val25 + val26 + val27 + val28 +
         val29;
}

float time_me29(uint64_t *bigarray, size_t howmanyhits, size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += naked_access_29(bigarray, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << 29 << " " << mintime << std::endl;
  return mintime;
}

template <size_t howmanylanes>
float time_me(uint64_t *bigarray, size_t length, size_t howmanyhits,
              size_t repeat) {
  clock_t begin_time, end_time;
  float mintime = 99999999999;
  uint64_t bogus = 0;
  while (repeat-- > 0) {
    begin_time = clock();
    bogus += access<howmanylanes>(bigarray, length, howmanyhits);
    end_time = clock();
    float tv = float(end_time - begin_time) / CLOCKS_PER_SEC;
    if (tv < mintime)
      mintime = tv;
  }
  if (bogus == 0x010101) {
    printf("ping!");
  }
  std::cout << howmanylanes << " " << mintime << std::endl;
  return mintime;
}

int measure(size_t length) {
  uint64_t *bigarray = (uint64_t *)malloc(sizeof(uint64_t) * length);
  for (size_t i = 0; i < length; i++)
    bigarray[i] = 3 * i - i + 1;
  float time_measure[15];
  size_t howmanyhits = 1 * 4 * 5 * 6 * 7 * 8 * 9 * 11 * 13;
  int repeat = 5;

  time_measure[1] = time_me<1>(bigarray, length, howmanyhits, repeat);
  time_measure[2] = time_me<2>(bigarray, length, howmanyhits, repeat);
  time_measure[3] = time_me<3>(bigarray, length, howmanyhits, repeat);
  time_measure[4] = time_me<4>(bigarray, length, howmanyhits, repeat);
  time_measure[5] = time_me<5>(bigarray, length, howmanyhits, repeat);
  time_measure[6] = time_me<6>(bigarray, length, howmanyhits, repeat);
  time_measure[7] = time_me<7>(bigarray, length, howmanyhits, repeat);
  time_measure[8] = time_me<8>(bigarray, length, howmanyhits, repeat);
  time_measure[9] = time_me<9>(bigarray, length, howmanyhits, repeat);
  time_measure[10] = time_me<10>(bigarray, length, howmanyhits, repeat);
  time_measure[11] = time_me<11>(bigarray, length, howmanyhits, repeat);
  time_measure[12] = time_me<12>(bigarray, length, howmanyhits, repeat);
  time_measure[13] = time_me<13>(bigarray, length, howmanyhits, repeat);
  time_measure[14] = time_me<14>(bigarray, length, howmanyhits, repeat);

  for (size_t i = 2; i <= 14; i++) {
    float ratio = (time_measure[i - 1] - time_measure[i]) / time_measure[i - 1];
    printf("gain in going from %zu lanes to %zu lanes is %.1f %% \n", i-1,i,ratio * 100.);
    if (ratio <
        0.05) // if a new lane does not add at least 5% of performance...
    {
      std::cout << "Maybe you have about " << i - 1 << " parallel paths? "
                << std::endl;
      return i - 1;
      break;
    }
  }
  return 10000;
}

int naked_measure(size_t length) {
  uint64_t *bigarray = (uint64_t *)malloc(sizeof(uint64_t) * length);
  for (size_t i = 0; i < length; i++)
    bigarray[i] = rand() % length; //((uint32_t)murmur64(i) * length) >> 32;
  float time_measure[30];
  size_t howmanyhits = 1 * 4 * 5 * 6 * 7 * 8 * 9 * 11 * 13;
  int repeat = 5;

  time_measure[1] = time_me1(bigarray, howmanyhits, repeat);
  time_measure[2] = time_me2(bigarray, howmanyhits, repeat);
  time_measure[3] = time_me3(bigarray, howmanyhits, repeat);
  time_measure[4] = time_me4(bigarray, howmanyhits, repeat);
  time_measure[5] = time_me5(bigarray, howmanyhits, repeat);
  time_measure[6] = time_me6(bigarray, howmanyhits, repeat);
  time_measure[7] = time_me7(bigarray, howmanyhits, repeat);
  time_measure[8] = time_me8(bigarray, howmanyhits, repeat);
  time_measure[9] = time_me9(bigarray, howmanyhits, repeat);
  time_measure[10] = time_me10(bigarray, howmanyhits, repeat);
  time_measure[11] = time_me11(bigarray, howmanyhits, repeat);
  time_measure[12] = time_me12(bigarray, howmanyhits, repeat);
  time_measure[13] = time_me13(bigarray, howmanyhits, repeat);
  time_measure[14] = time_me14(bigarray, howmanyhits, repeat);
  time_measure[15] = time_me15(bigarray, howmanyhits, repeat);
  time_measure[16] = time_me16(bigarray, howmanyhits, repeat);
  time_measure[17] = time_me17(bigarray, howmanyhits, repeat);
  time_measure[18] = time_me18(bigarray, howmanyhits, repeat);
  time_measure[19] = time_me19(bigarray, howmanyhits, repeat);
  time_measure[20] = time_me20(bigarray, howmanyhits, repeat);
  time_measure[21] = time_me21(bigarray, howmanyhits, repeat);
  time_measure[22] = time_me22(bigarray, howmanyhits, repeat);
  time_measure[23] = time_me23(bigarray, howmanyhits, repeat);
  time_measure[24] = time_me24(bigarray, howmanyhits, repeat);
  time_measure[25] = time_me25(bigarray, howmanyhits, repeat);
  time_measure[26] = time_me26(bigarray, howmanyhits, repeat);
  time_measure[27] = time_me27(bigarray, howmanyhits, repeat);
  time_measure[28] = time_me28(bigarray, howmanyhits, repeat);
  time_measure[29] = time_me29(bigarray, howmanyhits, repeat);

  for (size_t i = 2; i <= 29; i++) {
    float ratio = (time_measure[i - 1] - time_measure[i]) / time_measure[i - 1];

    if (ratio <
        0.01) // if a new lane does not add at least 1% of performance...
    {
      std::cout << "Maybe you have about " << i - 1 << " parallel paths? "
                << std::endl;
      return i - 1;
      break;
    }
  }
  return 10000;
}

int main() {
  size_t length = 1 << 25;
  for (size_t i = 0; i < 3; i++) {
    measure(length);
  }
}
