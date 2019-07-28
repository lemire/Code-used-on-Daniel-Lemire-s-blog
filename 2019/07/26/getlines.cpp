#include <istream>
#include <string>
#include <random>
#include <sstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <chrono>
#include <sys/time.h>
using namespace std;
using namespace std::chrono;

void rewind(std::istream & is) {
  is.clear();
  is.seekg(0,std::ios::beg);
}

__attribute__ ((noinline))
size_t sum_line_lengths(std::istream & is) {
  std::string line;
  size_t sumofalllinelengths{0};
  while(getline(is, line)) {
    sumofalllinelengths += line.size();
  }
  return sumofalllinelengths;
}

char * readall(const char * filename, size_t * size) {
    FILE *fp;
    long len;
    char *buffer = NULL;
    fp = fopen (filename, "rb" );
    if(!fp) return buffer;
    fseek( fp , 0L , SEEK_END);
    len = ftell( fp );
    *size = len;
    rewind( fp );
    buffer = (char*)malloc(len+1);
    if(buffer == NULL) {
       fclose(fp);
       return buffer;
    }
    if( 1!=fread(buffer , len, 1 , fp) ) {
       fclose(fp);
       free(buffer);
       return NULL;
    }
    fclose(fp);
    return buffer;
}





int main(int argc, char** argv) {
  if(argc < 2) return EXIT_FAILURE;
  size_t len;
  char * buffer = readall(argv[1], &len);
  if(buffer == NULL) return EXIT_FAILURE;
  std::string core (buffer);
  std::istringstream ss(core);
  size_t sum = 0;
  for (int k = 0; k < 19 ; k++) {
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    size_t c = sum_line_lengths(ss);
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    double dif = duration_cast<nanoseconds>( t2 - t1 ).count();
    double speed = len / (1024*1024*1024.0)  / (dif / 1000000000.0);
    sum += c;
    if(k >= 17) std::cout << "speed: " << speed << " GB/s" << std::endl;
    rewind(ss);
  }
  if(sum == 3123242) std::cout << sum << std::endl;
  free(buffer); 
  return EXIT_SUCCESS;
}
