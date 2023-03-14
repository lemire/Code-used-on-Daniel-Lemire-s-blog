#ifndef __ARM_FEATURE_SVE
#warning "We need SVE!"
#endif
#include "linux-perf-events.h"
#include <arm_sve.h>
#include <arm_neon.h>

#include <iostream>
#include <random>
#include <vector>

size_t trimspaces(const char *s, size_t len, char *out) {
  char * init_out{out};
  for(size_t i = 0; i < len; i++) {
    *out = s[i];
    out += (s[i] != ' ');
  }
  return out - init_out;
}

size_t sve_trimspaces(const char *s, size_t len, char *out) {
  uint8_t *out8 = reinterpret_cast<uint8_t *>(out);
  size_t i = 0;
  for (; i + svcntw() <= len; i += svcntw()) {
    svuint32_t input = svld1sb_u32(svptrue_b32(), (const int8_t *)s + i);
    svbool_t matches = svcmpne_n_u32(svptrue_b32(), input, 32);
    svuint32_t compressed = svcompact_u32(matches, input);
    svst1b_u32(svptrue_b32(), out8, compressed);
    out8 += svcntp_b32(svptrue_b32(), matches);
  }
  if (i < len) {
    svbool_t read_mask = svwhilelt_b32(i, len);
    svuint32_t input = svld1sb_u32(read_mask, (const int8_t *)s + i);
    svbool_t matches = svcmpne_n_u32(read_mask, input, 32);
    svuint32_t compressed = svcompact_u32(matches, input);
    svst1b_u32(read_mask, out8, compressed);
    out8 += svcntp_b32(read_mask, matches);
  }
  return out8 - reinterpret_cast<uint8_t *>(out);
}



size_t sve_trimspaces_unrolled(const char *s, size_t len, char *out) {
  uint8_t *out8 = reinterpret_cast<uint8_t *>(out);
  size_t i = 0;
  for (; i + 4*svcntw() <= len; i += 4*svcntw()) {
    svuint32_t input1 = svld1sb_u32(svptrue_b32(), (const int8_t *)s + i);
    svuint32_t input2 = svld1sb_u32(svptrue_b32(), (const int8_t *)s + i + svcntw());
    svuint32_t input3 = svld1sb_u32(svptrue_b32(), (const int8_t *)s + i + 2*svcntw());
    svuint32_t input4 = svld1sb_u32(svptrue_b32(), (const int8_t *)s + i + 3*svcntw());
    svbool_t matches1 = svcmpne_n_u32(svptrue_b32(), input1, 32);
    svbool_t matches2 = svcmpne_n_u32(svptrue_b32(), input2, 32);
    svbool_t matches3 = svcmpne_n_u32(svptrue_b32(), input3, 32);
    svbool_t matches4 = svcmpne_n_u32(svptrue_b32(), input4, 32);
    svuint32_t compressed1 = svcompact_u32(matches1, input1);
    svuint32_t compressed2 = svcompact_u32(matches2, input2);
    svuint32_t compressed3 = svcompact_u32(matches3, input3);
    svuint32_t compressed4 = svcompact_u32(matches4, input4);
    svst1b_u32(svptrue_b32(), out8, compressed1);
    out8 += svcntp_b32(svptrue_b32(), matches1);
    svst1b_u32(svptrue_b32(), out8, compressed2);
    out8 += svcntp_b32(svptrue_b32(), matches2);
    svst1b_u32(svptrue_b32(), out8, compressed3);
    out8 += svcntp_b32(svptrue_b32(), matches3);
    svst1b_u32(svptrue_b32(), out8, compressed4);
    out8 += svcntp_b32(svptrue_b32(), matches4);

  }
  for (; i + svcntw() <= len; i += svcntw()) {
    svuint32_t input = svld1sb_u32(svptrue_b32(), (const int8_t *)s + i);
    svbool_t matches = svcmpne_n_u32(svptrue_b32(), input, 32);
    svuint32_t compressed = svcompact_u32(matches, input);
    svst1b_u32(svptrue_b32(), out8, compressed);
    out8 += svcntp_b32(svptrue_b32(), matches);
  }
  if (i < len) {
    svbool_t read_mask = svwhilelt_b32(i, len);
    svuint32_t input = svld1sb_u32(read_mask, (const int8_t *)s + i);
    svbool_t matches = svcmpne_n_u32(read_mask, input, 32);
    svuint32_t compressed = svcompact_u32(matches, input);
    svst1b_u32(read_mask, out8, compressed);
    out8 += svcntp_b32(read_mask, matches);
  }
  return out8 - reinterpret_cast<uint8_t *>(out);
}

size_t sve_trimspaces_oneloop_alt(const char *s, size_t len, char *out) {
  uint8_t *out8 = reinterpret_cast<uint8_t *>(out);
  size_t i = 0;
  svbool_t all = svptrue_b32();
  svbool_t Pg;
  // see http://www.cse.iitm.ac.in/~rupesh/events/arm2021/CDAC%20-%20Overview%20of%20the%20Arm%20ISA%20for%20HPC.pdf
  for (; svptest_first(all,Pg=svwhilelt_b32(i, len)); i += svcntw()) {
    svbool_t read_mask = svwhilelt_b32(i, len);
    svuint32_t input = svld1sb_u32(read_mask, (const int8_t *)s + i);
    svbool_t matches = svcmpne_n_u32(read_mask, input, 32);
    svuint32_t compressed = svcompact_u32(matches, input);
    svst1b_u32(read_mask, out8, compressed);
    out8 += svcntp_b32(read_mask, matches);
  }
  return out8 - reinterpret_cast<uint8_t *>(out);
}


size_t sve_trimspaces_oneloop(const char *s, size_t len, char *out) {
  uint8_t *out8 = reinterpret_cast<uint8_t *>(out);
  size_t i = 0;
  for (; i < len; i += svcntw()) {
    svbool_t read_mask = svwhilelt_b32(i, len);
    svuint32_t input = svld1sb_u32(read_mask, (const int8_t *)s + i);
    svbool_t matches = svcmpne_n_u32(read_mask, input, 32);
    svuint32_t compressed = svcompact_u32(matches, input);
    svst1b_u32(read_mask, out8, compressed);
    out8 += svcntp_b32(read_mask, matches);
  }
  return out8 - reinterpret_cast<uint8_t *>(out);
}


#include "bigtable.h"

static inline uint8x16_t is_nonwhite(uint8x16_t data) {
  const uint8x16_t wchar = vdupq_n_u8(' ');
  const uint8x16_t allchar = vdupq_n_u8(0xFF);
  uint8x16_t isw = vceqq_u8(data, wchar);
  return  veorq_u8(isw, allchar); // negation
}
static inline uint16_t neonmovemask_addv(uint8x16_t input8) {
  uint16x8_t input = vreinterpretq_u16_u8(input8);
  const uint16x8_t bitmask = { 0x0101 , 0x0202, 0x0404, 0x0808, 0x1010, 0x2020, 0x4040, 0x8080 };
  uint16x8_t minput = vandq_u16(input, bitmask);
  return vaddvq_u16(minput);
}

static inline uint8_t bytepopcount(uint8x16_t v) {
  return vaddvq_u8(vshrq_n_u8(v,7));
}

static inline size_t neontbl_despace(const char *bytes, size_t howmany, char* out) {
  size_t i = 0, pos = 0;
  const size_t chunk_size = 16 * 4 * 1;
  for (; i + chunk_size <= howmany; i += chunk_size) {
    uint8x16_t vecbytes0 = vld1q_u8((uint8_t *)bytes + i);
    uint8x16_t vecbytes1 = vld1q_u8((uint8_t *)bytes + i + 16);
    uint8x16_t vecbytes2 = vld1q_u8((uint8_t *)bytes + i + 32);
    uint8x16_t vecbytes3 = vld1q_u8((uint8_t *)bytes + i + 48);
    // as early as possible, we compute the population counts
    uint8x16_t w0 = is_nonwhite(vecbytes0);
    uint8_t numberofkeptchars0 = bytepopcount(w0);
    uint8x16_t shuf0 = vld1q_u8(shufmask + 16 * neonmovemask_addv(w0));

    uint8x16_t reshuf0 = vqtbl1q_u8(vecbytes0,shuf0);
    uint8x16_t w1 = is_nonwhite(vecbytes1);
    uint8_t numberofkeptchars1 = bytepopcount(w1);
    uint8x16_t shuf1 = vld1q_u8(shufmask + 16 * neonmovemask_addv(w1));
    uint8x16_t reshuf1 = vqtbl1q_u8(vecbytes1,shuf1);
    uint8x16_t w2 = is_nonwhite(vecbytes2);
    uint8_t numberofkeptchars2 = bytepopcount(w2);
    uint8x16_t shuf2 = vld1q_u8(shufmask + 16 * neonmovemask_addv(w2));
    uint8x16_t reshuf2 = vqtbl1q_u8(vecbytes2,shuf2);
    uint8x16_t w3 = is_nonwhite(vecbytes3);
    uint8_t numberofkeptchars3 = bytepopcount(w3);
    uint8x16_t shuf3 = vld1q_u8(shufmask + 16 * neonmovemask_addv(w3));
    uint8x16_t reshuf3 = vqtbl1q_u8(vecbytes3,shuf3);

    vst1q_u8((uint8_t *)out + pos,reshuf0);
    pos += numberofkeptchars0;

    vst1q_u8((uint8_t *)out + pos,reshuf1);
    pos += numberofkeptchars1;

    vst1q_u8((uint8_t *)out + pos,reshuf2);
    pos += numberofkeptchars2;

    vst1q_u8((uint8_t *)out + pos,reshuf3);
    pos += numberofkeptchars3;

  }
  while (i < howmany) {
    const unsigned char c = bytes[i++];
    out[pos] = c;
    pos += (c != 32);
  }
  return pos;
}

void demo() {
  size_t N = 10000;
  char * input = new char[N];
  char * output = new char[N];
  for(size_t i = 0; i < N; i++) {
    input[i] = rand();
  }
  LinuxEvents<PERF_TYPE_HARDWARE> linux_events(std::vector<int>{
      PERF_COUNT_HW_CPU_CYCLES,
      PERF_COUNT_HW_INSTRUCTIONS,
  });
  volatile size_t len{0};
  std::cout << "scalar" << std::endl;
  std::vector<unsigned long long> results(2);

  linux_events.start();
  len = trimspaces(input, N, output);
  linux_events.end(results);
  std::cout << len << std::endl;
  std::cout << "cycles/bytes " << double(results[0]) / (len) << " ";
  std::cout << "instructions/bytes " << double(results[1]) / (len)
            << " ";
  std::cout << "instructions/cycle " << double(results[1]) / results[0]
            << std::endl;
  std::cout << std::endl;


  std::cout << "neon" << std::endl;

  linux_events.start();
  len = neontbl_despace(input, N, output);
  linux_events.end(results);
  std::cout << len << std::endl;

  std::cout << "cycles/bytes " << double(results[0]) / (len) << " ";
  std::cout << "instructions/bytes " << double(results[1]) / (len)
            << " ";
  std::cout << "instructions/cycle " << double(results[1]) / results[0]
            << std::endl;
  std::cout << std::endl;


  std::cout << "sve one loop" << std::endl;

  linux_events.start();
  len = sve_trimspaces_oneloop(input, N, output);
  linux_events.end(results);
  std::cout << len << std::endl;

  std::cout << "cycles/bytes " << double(results[0]) / (len) << " ";
  std::cout << "instructions/bytes " << double(results[1]) / (len)
            << " ";
  std::cout << "instructions/cycle " << double(results[1]) / results[0]
            << std::endl;
  std::cout << std::endl;

  std::cout << "sve one loop alt" << std::endl;

  linux_events.start();
  len = sve_trimspaces_oneloop_alt(input, N, output);
  linux_events.end(results);
  std::cout << len << std::endl;

  std::cout << "cycles/bytes " << double(results[0]) / (len) << " ";
  std::cout << "instructions/bytes " << double(results[1]) / (len)
            << " ";
  std::cout << "instructions/cycle " << double(results[1]) / results[0]
            << std::endl;
  std::cout << std::endl;


  std::cout << "sve" << std::endl;

  linux_events.start();
  len = sve_trimspaces(input, N, output);
  linux_events.end(results);
  std::cout << len << std::endl;

  std::cout << "cycles/bytes " << double(results[0]) / (len) << " ";
  std::cout << "instructions/bytes " << double(results[1]) / (len)
            << " ";
  std::cout << "instructions/cycle " << double(results[1]) / results[0]
            << std::endl;
  std::cout << std::endl;

  std::cout << "sve unrolled" << std::endl;

  linux_events.start();
  len = sve_trimspaces_unrolled(input, N, output);
  linux_events.end(results);
  std::cout << len << std::endl;

  std::cout << "cycles/bytes " << double(results[0]) / (len) << " ";
  std::cout << "instructions/bytes " << double(results[1]) / (len)
            << " ";
  std::cout << "instructions/cycle " << double(results[1]) / results[0]
            << std::endl;
  std::cout << std::endl;
}

int main() {
  printf("===\n");
  for(size_t i = 0; i < 5; i++) {
    demo();
  }
  return EXIT_SUCCESS;
}
