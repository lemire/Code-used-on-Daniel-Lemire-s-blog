// The Computer Language Benchmarks Game
// http://benchmarksgame.alioth.debian.org/
//
// Contributed by Jeremy Zerfas

// This controls the width of lines that are output by this program.
#define MAXIMUM_LINE_WIDTH   60

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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


// Print a pseudorandom DNA sequence that is number_Of_Characters_To_Create
// characters long and made up of the nucleotides specified in
// nucleotides_Information and occurring at the frequencies specified in
// nucleotides_Information. The output is also wrapped to MAXIMUM_LINE_WIDTH
// columns.
static void generate_And_Wrap_Pseudorandom_DNA_Sequence(
  const nucleotide_info nucleotides_Information[],
  const intnative_t number_Of_Nucleotides,
  const intnative_t number_Of_Characters_To_Create){

   // Cumulate the probabilities. Note that the probability is being multiplied
   // by IM because later on we'll also be calling the random number generator
   // with a value that is multiplied by IM. Since the random number generator
   // does a division by IM this allows the compiler to cancel out the
   // multiplication and division by IM with each other without requiring any
   // changes to the random number generator code whose code was explicitly
   // defined in the rules.
   float cumulative_Probabilities[number_Of_Nucleotides],
     cumulative_Probability=0.0;
   for(intnative_t i=0; i<number_Of_Nucleotides; i++){
      cumulative_Probability+=nucleotides_Information[i].probability;
      cumulative_Probabilities[i]=cumulative_Probability*IM;
   }

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

      // Fill up the line with characters from nucleotides_Information[] that
      // are selected by looking up a pseudorandom number.
      for(intnative_t column=0; column<line_Length; column++){
         const float r=get_LCG_Pseudorandom_Number(IM);

         // Count the number of nucleotides with a probability less than what
         // was selected by the random number generator and then use that
         // count as an index for the nucleotide to select. It's arguable
         // whether this qualifies as a linear search but I guess you can say
         // that you're doing a linear search for all the nucleotides with a
         // probability less than what was selected by the random number
         // generator and then just counting how many matches were found.
         // With a small number of nucleotides this can be faster than doing
         // a more normal linear search (although in some cases it may
         // generate different results) and a couple of the other programs
         // already do this as well so we will too.
         intnative_t count=0;
         for(intnative_t i=0; i<number_Of_Nucleotides; i++)
            if(cumulative_Probabilities[i]<=r)
               count++;

         line[column]=nucleotides_Information[count].letter;
      }

      // Output the line to stdout and update the
      // current_Number_Of_Characters_To_Create.
      fwrite(line, line_Length+1, 1, stdout);
      current_Number_Of_Characters_To_Create-=line_Length;
   }
}


int main(int argc, char ** argv){
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
