#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <time.h>

size_t M = 1000000;


int compute_two(char * array, size_t N, int * random) {
  int answer = 0;
  for(size_t i = 0; i < 2*M; i+= 2) {
    answer += array[random[i]] ^ array[random[i + 1]];
  }
  return answer;
}

int compute_two_plus(char * array, size_t N, int * random) {
  int answer = 0;
  for(size_t i = 0; i < 2*M; i+= 2) {
    int idx1 = random[i];
    int idx2 = random[i + 1];
    
    answer += array[idx1] ^ array[idx1 + 1] 
           ^ array[idx2]  ^ array[idx2 + 1];
  }
  return answer;
}

int compute_three(char * array, size_t N, int * random) {
  int answer = 0;
  for(size_t i = 0; i < 3*M; i+= 3) {
    answer += array[random[i]] ^ array[random[i + 1]] 
              ^ array[random[i + 2]];
  }
  return answer;
}

int main(int argc, const char ** val) {
  size_t N = 100000000;
  if(argc > 1) {
    N = atoll(val[1]);
  }
  printf("N = %zu, %.1f MB\n", N, N/(1024.*1024));
  char* array = (char*) malloc(N);
  for(size_t i = 0; i < N; i++) { array[i] = i; }
  int* random = (int*) malloc(M * sizeof(int) * 3);
  for(size_t i = 0; i < 3 * M; i++) { 
    random[i] = rand() % (N-1); 
  }
  struct timespec start, stop;
  int answer = 0;
  size_t total_trials = 100;

  size_t ns2 = 0;
  size_t ns2p = 0;
  size_t ns3 = 0;
  for(size_t trial = 0; trial < total_trials; trial++) {
    clock_gettime( CLOCK_REALTIME, &start);
    answer += compute_two(array, N, random);
    clock_gettime( CLOCK_REALTIME, &stop);
    ns2 += (stop.tv_sec - start.tv_sec) * 1000000000 + ( stop.tv_nsec - start.tv_nsec );

    clock_gettime( CLOCK_REALTIME, &start);
    answer += compute_two_plus(array, N, random);
    clock_gettime( CLOCK_REALTIME, &stop);
    ns2p += (stop.tv_sec - start.tv_sec) * 1000000000 + ( stop.tv_nsec - start.tv_nsec );
 
    clock_gettime( CLOCK_REALTIME, &start);
    answer += compute_three(array, N, random);
    clock_gettime( CLOCK_REALTIME, &stop);
    ns3 += (stop.tv_sec - start.tv_sec) * 1000000000 + ( stop.tv_nsec - start.tv_nsec );
  }
  printf("two  : %.1f ns\n", (double)ns2 / M / total_trials);
  printf("two+  : %.1f ns\n", (double)ns2p / M / total_trials);
  printf("three: %.1f ns\n", (double)ns3 / M / total_trials);

  printf("bogus %d \n", answer);

  return EXIT_SUCCESS;
}
