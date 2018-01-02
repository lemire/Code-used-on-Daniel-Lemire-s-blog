// The Computer Language Benchmarks Game
// http://benchmarksgame.alioth.debian.org/
//
// Contributed by Jeremy Zerfas
// MODIFIED BY D. Lemire (AVX2 vectorization)

// This controls the width of lines that are output by this program.
#define MAXIMUM_LINE_WIDTH   60
#include <x86intrin.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// intptr_t should be the native integer type on most sane systems.
typedef intptr_t intnative_t;

typedef struct{
   char letter;
   float probability;
} nucleotide_info;


// Repeatedly print string_To_Repeat until it has printed
// number_Of_Characters_To_Create. The output is also wrapped to
// MAXIMUM_LINE_WIDTH columns.
static void repeat_And_Wrap_String(const char string_To_Repeat[],
  const intnative_t number_Of_Characters_To_Create){
   const intnative_t string_To_Repeat_Length=strlen(string_To_Repeat);

   // Create an extended_String_To_Repeat which is a copy of string_To_Repeat
   // but extended with another copy of the first MAXIMUM_LINE_WIDTH characters
   // of string_To_Repeat appended to the end. Later on this allows us to
   // generate a line of output just by doing simple memory copies using an
   // appropriate offset into extended_String_To_Repeat.
   char extended_String_To_Repeat[string_To_Repeat_Length+MAXIMUM_LINE_WIDTH];
   for(intnative_t column=0; column<string_To_Repeat_Length+MAXIMUM_LINE_WIDTH;
     column++)
      extended_String_To_Repeat[column]=
        string_To_Repeat[column%string_To_Repeat_Length];
   intnative_t offset=0;

   char line[MAXIMUM_LINE_WIDTH+1];
   line[MAXIMUM_LINE_WIDTH]='\n';

   for(intnative_t current_Number_Of_Characters_To_Create=
     number_Of_Characters_To_Create;
     current_Number_Of_Characters_To_Create>0;){
      // Figure out the length of the line we need to write. If it's less than
      // MAXIMUM_LINE_WIDTH then we also need to add a line feed in the right
      // spot too.
      intnative_t line_Length=MAXIMUM_LINE_WIDTH;
      if(current_Number_Of_Characters_To_Create<MAXIMUM_LINE_WIDTH){
         line_Length=current_Number_Of_Characters_To_Create;
         line[line_Length]='\n';
      }

      memcpy(line, extended_String_To_Repeat+offset, line_Length);

      // Update the offset, reducing it by string_To_Repeat_Length if
      // necessary.
      offset+=line_Length;
      if(offset>string_To_Repeat_Length)
         offset-=string_To_Repeat_Length;

      // Output the line to stdout and update the
      // current_Number_Of_Characters_To_Create.
      fwrite(line, line_Length+1, 1, stdout);
      current_Number_Of_Characters_To_Create-=line_Length;
   }
}


// Generate a floating point pseudorandom number from 0.0 to max using a linear
// congruential generator.
#define IM      139968
#define IA      3877
#define IC      29573
#define SEED   42
static inline float get_LCG_Pseudorandom_Number(const float max){
   static uint32_t seed=SEED;
   seed=(seed*IA + IC)%IM;
   return max/IM*seed;
}

// computes the 32-bit integer division with IM
static __m256i divIM(__m256i a) {
      __m256i multiplier = _mm256_set1_epi64x(UINT64_C(502748801));
      __m256i fullmulhi32 = _mm256_mul_epu32(_mm256_srli_epi64(a, 32),multiplier);
      __m256i fullmullo32 = _mm256_mul_epu32(a, multiplier);
      fullmulhi32 = _mm256_and_si256(_mm256_srli_epi64(fullmulhi32,14), _mm256_set1_epi64x(UINT64_C(0xffffffff00000000)));
      fullmullo32 = _mm256_srli_epi64(fullmullo32,14+32);
      return _mm256_or_si256(fullmullo32, fullmulhi32);
}

// computes the 32-bit integer remainder with IM
static __m256i modIM(__m256i a) {
      return _mm256_sub_epi32(a,_mm256_mullo_epi32(divIM(a),_mm256_set1_epi32(IM)));
}


static __m256i vseed;

void init_vector_LCG() {
   // this is naive
   uint32_t buffer [8];
   for(int k = 0; k < 8 ; k++) buffer[k] = rand();
   vseed = _mm256_loadu_si256((__m256i*)buffer);
}

// vectorized version
static inline __m256i vector_get_LCG_Pseudorandom_Number(){
   vseed = modIM(_mm256_add_epi32(_mm256_mullo_epi32(vseed,_mm256_set1_epi32(IA)),_mm256_set1_epi32(IC)));
   return vseed;
}

// Print a pseudorandom DNA sequence that is number_Of_Characters_To_Create
// characters long and made up of the nucleotides specified in
// nucleotides_Information and occurring at the frequencies specified in
// nucleotides_Information. The output is also wrapped to MAXIMUM_LINE_WIDTH
// columns.
static void generate_And_Wrap_Pseudorandom_DNA_Sequence(
  const nucleotide_info nucleotides_Information[],
  const intnative_t number_Of_Nucleotides,
  const intnative_t number_Of_Characters_To_Create){
    uint8_t charbuffer [sizeof(__m256i)];
    assert(number_Of_Nucleotides<16);
    for(intnative_t i=0; i<number_Of_Nucleotides; i++){
      charbuffer[i] = nucleotides_Information[i].letter;
      charbuffer[i+sizeof(__m128i)] = nucleotides_Information[i].letter;
    }
    for(intnative_t i=number_Of_Nucleotides; i<16; i++){
      charbuffer[i] = '*';
      charbuffer[i+sizeof(__m128i)] = '*';
    }

    __m256i vletters = _mm256_loadu_si256((__m256i*)charbuffer);


   // Cumulate the probabilities. Note that the probability is being multiplied
   // by IM because later on we'll also be calling the random number generator
   // with a value that is multiplied by IM. Since the random number generator
   // does a division by IM this allows the compiler to cancel out the
   // multiplication and division by IM with each other without requiring any
   // changes to the random number generator code whose code was explicitly
   // defined in the rules.
   __m256i cumulative_Probabilities[number_Of_Nucleotides - 1];
   double cumulative_Probability=0.0;
   for(intnative_t i=0; i<number_Of_Nucleotides - 1; i++){
      cumulative_Probability+=nucleotides_Information[i].probability;
      cumulative_Probabilities[i]=_mm256_set1_epi32((uint32_t)(cumulative_Probability*IM));
   }

   char line[(MAXIMUM_LINE_WIDTH+1+sizeof(__m256i)-1)/sizeof(__m256i) * sizeof(__m256i)];
   line[MAXIMUM_LINE_WIDTH]='\n';

   for(intnative_t current_Number_Of_Characters_To_Create=
     number_Of_Characters_To_Create;
     current_Number_Of_Characters_To_Create>0;){
      intnative_t line_Length=MAXIMUM_LINE_WIDTH;
      if(current_Number_Of_Characters_To_Create<MAXIMUM_LINE_WIDTH){
         line_Length=current_Number_Of_Characters_To_Create;

      }
      for(intnative_t column=0; column<line_Length; column+=sizeof(__m256i)){
         const __m256i r1=vector_get_LCG_Pseudorandom_Number();
         const __m256i r2=vector_get_LCG_Pseudorandom_Number();
         const __m256i r3=vector_get_LCG_Pseudorandom_Number();
         const __m256i r4=vector_get_LCG_Pseudorandom_Number();
         __m256i count1 = _mm256_set1_epi32(number_Of_Nucleotides -1);
         __m256i count2 = _mm256_set1_epi32(number_Of_Nucleotides -1);
         __m256i count3 = _mm256_set1_epi32(number_Of_Nucleotides -1);
         __m256i count4 = _mm256_set1_epi32(number_Of_Nucleotides -1);
         for(intnative_t i=0; i<number_Of_Nucleotides -1 ; i++) {
            count1 = _mm256_add_epi32(count1,_mm256_cmpgt_epi32(cumulative_Probabilities[i],r1));
            count2 = _mm256_add_epi32(count2,_mm256_cmpgt_epi32(cumulative_Probabilities[i],r2));
            count3 = _mm256_add_epi32(count3,_mm256_cmpgt_epi32(cumulative_Probabilities[i],r3));
            count4 = _mm256_add_epi32(count4,_mm256_cmpgt_epi32(cumulative_Probabilities[i],r4));
         }
         __m256i index1 = _mm256_or_si256(count1,_mm256_slli_epi32(count2,8));
         __m256i index2 = _mm256_or_si256(_mm256_slli_epi32(count3,16), _mm256_slli_epi32(count3,24));
         __m256i index = _mm256_or_si256(index1,index2);
         __m256i vline = _mm256_shuffle_epi8(vletters,index);
         _mm256_storeu_si256((__m256i*)(line+column),vline);
      }
      line[line_Length]='\n';

      // Output the line to stdout and update the
      // current_Number_Of_Characters_To_Create.
      fwrite(line, line_Length+1, 1, stdout);
      current_Number_Of_Characters_To_Create-=line_Length;
   }
}


int main(int argc, char ** argv){
   init_vector_LCG();
   const intnative_t n=atoi(argv[1]);
   fputs(">ONE Homo sapiens alu\n", stdout);
   const char homo_Sapiens_Alu[]=
     "GGCCGGGCGCGGTGGCTCACGCCTGTAATCCCAGCACTTTGGGAGGCCGAGGCGGGCGGATCACCTGAGGTC"
     "AGGAGTTCGAGACCAGCCTGGCCAACATGGTGAAACCCCGTCTCTACTAAAAATACAAAAATTAGCCGGGCG"
     "TGGTGGCGCGCGCCTGTAATCCCAGCTACTCGGGAGGCTGAGGCAGGAGAATCGCTTGAACCCGGGAGGCGG"
     "AGGTTGCAGTGAGCCGAGATCGCGCCACTGCACTCCAGCCTGGGCGACAGAGCGAGACTCCGTCTCAAAAA";
   repeat_And_Wrap_String(homo_Sapiens_Alu, 2*n);

   fputs(">TWO IUB ambiguity codes\n", stdout);
   nucleotide_info iub_Nucleotides_Information[]={
     {'a', 0.27}, {'c', 0.12}, {'g', 0.12}, {'t', 0.27}, {'B', 0.02},
     {'D', 0.02}, {'H', 0.02}, {'K', 0.02}, {'M', 0.02}, {'N', 0.02},
     {'R', 0.02}, {'S', 0.02}, {'V', 0.02}, {'W', 0.02}, {'Y', 0.02}};
   generate_And_Wrap_Pseudorandom_DNA_Sequence(iub_Nucleotides_Information,
     sizeof(iub_Nucleotides_Information)/sizeof(nucleotide_info), 3*n);

   fputs(">THREE Homo sapiens frequency\n", stdout);
   nucleotide_info homo_Sapien_Nucleotides_Information[]={
     {'a', 0.3029549426680}, {'c', 0.1979883004921},
     {'g', 0.1975473066391}, {'t', 0.3015094502008}};
   generate_And_Wrap_Pseudorandom_DNA_Sequence(
     homo_Sapien_Nucleotides_Information,
     sizeof(homo_Sapien_Nucleotides_Information)/sizeof(nucleotide_info), 5*n);

   return 0;
}
