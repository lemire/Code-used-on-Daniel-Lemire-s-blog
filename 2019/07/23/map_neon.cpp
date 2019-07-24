#include <arm_neon.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "benchmark.h"
__attribute__ ((noinline))
void transform(const uint8_t * map, uint8_t * values, size_t volume) {
  for(size_t i = 0; i < volume; i++) {
    values[i] = map[values[i]];
  } 
}

static inline uint8x16_t simd_transform16(uint8x16x4_t * table, uint8x16_t input) {
  uint8x16_t  t1 = vqtbl4q_u8(table[0],  input);
  uint8x16_t  t2 = vqtbl4q_u8(table[1],  veorq_u8(input, vdupq_n_u8(0x40)));
  uint8x16_t  t3 = vqtbl4q_u8(table[2],  veorq_u8(input, vdupq_n_u8(0x80)));
  uint8x16_t  t4 = vqtbl4q_u8(table[3],  veorq_u8(input, vdupq_n_u8(0xc0)));
  return vorrq_u8(vorrq_u8(t1,t2), vorrq_u8(t3,t4));
}

static inline uint8x16_t simd_transform16x(uint8x16x4_t * table, uint8x16_t input) {
  uint8x16_t  t1 = vqtbx4q_u8(input, table[0],  input);
  t1 = vqtbx4q_u8(t1, table[1],  veorq_u8(input, vdupq_n_u8(0x40)));
  t1 = vqtbx4q_u8(t1, table[2],  veorq_u8(input, vdupq_n_u8(0x80)));
  t1 = vqtbx4q_u8(t1, table[3],  veorq_u8(input, vdupq_n_u8(0xc0)));
  return t1;
}
static inline uint8x16_t simd_transform16_ascii(uint8x16x4_t * table, uint8x16_t input) {
  uint8x16_t  t1 = vqtbl4q_u8(table[0],  input);
  uint8x16_t  t2 = vqtbl4q_u8(table[1],  veorq_u8(input, vdupq_n_u8(0x40)));
  return vorrq_u8(t1,t2);
}


// should be vld1q_u8_x4 but it is not supported everywhere 
static inline uint8x16x4_t neon_load4(const uint8_t * map)  {
  uint8x16x4_t answer;
  answer.val[0] = vld1q_u8(map +  0);
  answer.val[1] = vld1q_u8(map + 16);
  answer.val[2] = vld1q_u8(map + 32);
  answer.val[3] = vld1q_u8(map + 48);
  return answer;
}

__attribute__ ((noinline))
void neon_transform(const uint8_t * map, uint8_t * values, size_t volume) {
  uint8x16x4_t table[4];
  table[0] = neon_load4(map);
  table[1] = neon_load4(map + 0x40);
  table[2] = neon_load4(map + 0x80);
  table[3] = neon_load4(map + 0xc0);
  size_t i = 0;
  for(;i + 16 <=  volume; i+= 16) {
    vst1q_u8(values + i, simd_transform16(table,vld1q_u8(values + i)));
  }
  for(; i < volume; i++) {
    values[i] = map[values[i]];
  } 
}

__attribute__ ((noinline))
void neon_transformx(const uint8_t * map, uint8_t * values, size_t volume) {
  uint8x16x4_t table[4];
  table[0] = neon_load4(map);
  table[1] = neon_load4(map + 0x40);
  table[2] = neon_load4(map + 0x80);
  table[3] = neon_load4(map + 0xc0);
  size_t i = 0;
  for(;i + 16 <=  volume; i+= 16) {
    vst1q_u8(values + i, simd_transform16x(table,vld1q_u8(values + i)));
  }
  for(; i < volume; i++) {
    values[i] = map[values[i]];
  } 
}

__attribute__ ((noinline))
void neon_transform_ascii(const uint8_t * map, uint8_t * values, size_t volume) {
  uint8x16x4_t table[2];
  table[0] = neon_load4(map);
  table[1] = neon_load4(map + 0x40);
  size_t i = 0;
  for(;i + 16 <=  volume; i+= 16) {
    vst1q_u8(values + i, simd_transform16_ascii(table,vld1q_u8(values + i)));
  }
  for(; i < volume; i++) {
    values[i] = map[values[i]];
  } 
}

bool test() {
  uint8_t map[256];
  for(size_t i = 0; i < 256; i++) map[i] = (i ^ 127);
  uint8_t values1[256]; 
  for(size_t i = 0; i < 256; i++) values1[i] = i;
  uint8_t values2[256]; 
  for(size_t i = 0; i < 256; i++) values2[i] = i;
  transform(map, values1, 256);
  neon_transform(map, values2, 256);
  for(size_t i = 0; i < 256; i++) 
     if(values2[i] != values1[i]) return false;
  for(size_t i = 0; i < 256; i++) values2[i] = i;
  neon_transformx(map, values2, 256);
  for(size_t i = 0; i < 256; i++) 
     if(values2[i] != values1[i]) return false;
  return true;
}

void demo() {
  uint8_t map[256];
  for(size_t i = 0; i < 256; i++) map[i] = (i ^ 127);
  size_t volume = 4096;
  uint8_t * values = (uint8_t*)malloc(volume);
  for(size_t i = 0; i < 256; i++) values[i] = (i % 256);
  int repeat = 5;
  BEST_TIME_NS(transform(map, values,volume), , repeat, volume, true);
  BEST_TIME_NS(neon_transform(map, values,volume), , repeat, volume, true);
  BEST_TIME_NS(neon_transformx(map, values,volume), , repeat, volume, true);
  BEST_TIME_NS(neon_transform_ascii(map, values,volume), , repeat, volume, true);
    
  free(values);
}

int main() {
  if(!test()) {
    printf("bug\n");
    return EXIT_FAILURE;
  }
  printf("Test ok.\n"); 
  demo();
  return EXIT_SUCCESS;
}

