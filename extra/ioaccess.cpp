#include <fcntl.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/types.h>
 #include <sys/time.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>


using namespace std;


// write N * 512 integers
void fillFile(int N, char * name) {
	FILE * fd = ::fopen(name, "wb");
    if (fd == NULL) {
      cerr << "problem" << endl;
      return;
    }
    int numbers[512];
	for(int k = 0; k < 512; ++k) 
	  numbers[k] = k; // whatever
	for(int t = 0; t < N; ++t) {
		int size = 512;
		if(fwrite(&size, sizeof(int),1,fd)!=1) {
		  cout<<"Data can't be written???"<<endl;
		  return;
		}		
		if(fwrite(&numbers[0], sizeof(int),512,fd)!=512) {
		  cout<<"Data can't be written???"<<endl;
		  return;
		}
	}
	::fclose(fd);
}

int doSomeComputation(int * numbers, size_t size) {
	int answer = 0;
	// totally arbitrary
	for(size_t k = 0; k < size; k+= 2) {
		answer += numbers[k];
	}
	for(size_t k = 1; k < size; k+= 2) {
		answer += numbers[k] * 2;
	}
	return answer;
}

int testfread(char * name, int N) {
	int answer = 0;
	FILE * fd = ::fopen(name, "rb");
    if (fd == NULL) {
      cerr << "problem" << endl;
      return -1;
    }
    vector<int> numbers(512);
	for(int t = 0; t < N; ++t) {
		int size = 0;
		if(fread(&size, sizeof(int),1,fd)!=1) {
		  cout<<"Data can't be read???"<<endl;
		  return -1;
		}
		numbers.resize(size);
		if(fread(&numbers[0], sizeof(int),numbers.size(),fd)!=numbers.size()) {
		  cout<<"Data can't be read???"<<endl;
		  return -1;
		}
		answer += doSomeComputation(&numbers[0],numbers.size());
	}
	::fclose(fd);
	return answer;
}


int testwithCpp(char * name, int N) {
	int answer = 0;
	ifstream in(name,ios::binary);
    vector<int> numbers(512);
	for(int t = 0; t < N; ++t) {
		int size = 0;
		in.read(reinterpret_cast<char *>(&size),sizeof(int));
		numbers.resize(size);
		in.read(reinterpret_cast<char *>(&numbers[0]),sizeof(int)*size);
		answer += doSomeComputation(&numbers[0],numbers.size());
	}
	in.close();
	return answer;
}

int testfreadwithsetbuffer(char * name, int N) {
	int answer = 0;
	FILE * fd = ::fopen(name, "rb");
	setvbuf (fd , NULL , _IOFBF , 1024*4 ); // large buffer
    if (fd == NULL) {
      cerr << "problem" << endl;
      return -1;
    }
    vector<int> numbers(512);
	for(int t = 0; t < N; ++t) {
		int size = 0;
		if(fread(&size, sizeof(int),1,fd)!=1) {
		  cout<<"Data can't be read???"<<endl;
		  return -1;
		}
		numbers.resize(size);
		if(fread(&numbers[0], sizeof(int),numbers.size(),fd)!=numbers.size()) {
		  cout<<"Data can't be read???"<<endl;
		  return -1;
		}
		answer += doSomeComputation(&numbers[0],numbers.size());
	}
	::fclose(fd);
	return answer;
}


int testfreadwithlargebuffer(char * name, int N) {
	int answer = 0;
	FILE * fd = ::fopen(name, "rb");
	setvbuf (fd , NULL , _IOFBF , 1024*1024*32 ); // large buffer
    if (fd == NULL) {
      cerr << "problem" << endl;
      return -1;
    }
    vector<int> numbers(512);
	for(int t = 0; t < N; ++t) {
		int size = 0;
		if(fread(&size, sizeof(int),1,fd)!=1) {
		  cout<<"Data can't be read???"<<endl;
		  return -1;
		}
		numbers.resize(size);
		if(fread(&numbers[0], sizeof(int),numbers.size(),fd)!=numbers.size()) {
		  cout<<"Data can't be read???"<<endl;
		  return -1;
		}
		answer += doSomeComputation(&numbers[0],numbers.size());
	}
	::fclose(fd);
	return answer;
}

int testwmmap(char * name, int N) {
	int answer = 0;
	int fd = ::open(name, O_RDONLY);
	size_t length = N * (512 + 1) * 4;
    int *  addr = reinterpret_cast<int *>(mmap(NULL, length, PROT_READ, MAP_FILE | MAP_PRIVATE, fd, 0));
int * initaddr = addr;    
if (addr == MAP_FAILED) {
    	  cout<<"Data can't be mapped???"<<endl;
		  return -1;
   }
   close(fd);
   vector<int> numbers(512);
   for(int t = 0; t < N; ++t) {
		int size = *addr++;
		numbers.resize(size);
		answer += doSomeComputation(addr,size);
		addr+=size;
	}
	munmap(initaddr,length);
	return answer;
}

int testread(char * name, int N) {
	int answer = 0;
	int fd = ::open(name, O_RDONLY);
    if (fd < 0) {
      cerr << "problem" << endl;
      return -1;
    }
    vector<int> numbers(512);
	for(int t = 0; t < N; ++t) {
		int size = 0;
		if(read(fd,&size, sizeof(int))!=sizeof(int)) {
		  cout<<"Data can't be read???"<<endl;
		  return -1;
		}
		numbers.resize(size);
		if(read(fd,&numbers[0], sizeof(int)*numbers.size())!=sizeof(int)*numbers.size()) {
		  cout<<"Data can't be read???"<<endl;
		  return -1;
		}
		answer += doSomeComputation(&numbers[0],numbers.size());
	}
	::close(fd);
	return answer;
}

class WallClockTimer {
public:
    struct timeval t1, t2;
    WallClockTimer() :
        t1(), t2() {
        gettimeofday(&t1, 0);
        t2 = t1;
    }
    void reset() {
        gettimeofday(&t1, 0);
        t2 = t1;
    }
    unsigned long long elapsed() {
        return ((t2.tv_sec - t1.tv_sec) * 1000ULL * 1000ULL) + ((t2.tv_usec - t1. tv_usec));
    }
    unsigned long long split() {
        gettimeofday(&t2, 0);
        return elapsed();
    }
};

class CPUTimer {
public:
    //clock_t t1, t2;
    struct rusage t1,t2;

    CPUTimer() :
        t1(), t2() {
        getrusage(RUSAGE_SELF, &t1);
        //t1 = clock();
        t2 = t1;
    }
    void reset() {
        getrusage(RUSAGE_SELF, &t1);
        //t1 = clock();
        t2 = t1;
    }
    // proxy for userelapsed
    unsigned long long elapsed() {
        return totalelapsed();//userelapsed();
    }

    unsigned long long totalelapsed() {
        return userelapsed() + systemelapsed();
    }
    // returns the *user* CPU time in micro seconds (mu s)
    unsigned long long userelapsed() {
        return ((t2.ru_utime.tv_sec - t1.ru_utime.tv_sec) * 1000ULL * 1000ULL) + ((t2.ru_utime.tv_usec - t1.ru_utime.tv_usec)
                );
    }

    // returns the *user* CPU time in micro seconds (mu s)
    unsigned long long systemelapsed() {
        return ((t2.ru_utime.tv_sec - t1.ru_utime.tv_sec) * 1000ULL * 1000ULL) + ((t2.ru_utime.tv_usec - t1.ru_utime.tv_usec)
                );
    }


    unsigned long long split() {
        getrusage(RUSAGE_SELF, &t2);
        return elapsed();
    }
};

int main() {
	const int N = 16384*32;
	int tot = 0;
	CPUTimer cput;
        WallClockTimer wct;
	for(int T = 0; T<30;++T) {
	   char * name =  tmpnam (NULL);// unsafe but for these purposes, ok
	  fillFile(N,name);

	  cout<<endl;
	  // don't report times
	  tot += testread(name,N);


	  // fread
	  cput.reset();wct.reset();
	  for(int x = 0; x<10; ++x) tot += testfread(name,N);
	  cout<<"fread\t\t\t"<<512*N*1.0/cput.split()<<" "<<512*N*1.0/wct.split()<<endl;
	  
	  // fread with set buffer
	  cput.reset();wct.reset();
	  for(int x = 0; x<10; ++x) tot += testfreadwithsetbuffer(name,N);
	  cout<<"fread w sbuffer\t\t"<<512*N*1.0/cput.split()<<" "<<512*N*1.0/wct.split()<<endl;
	  
	  // fread with large buffer
	  cput.reset();wct.reset();
	  for(int x = 0; x<10; ++x) tot += testfreadwithlargebuffer(name,N);
	  cout<<"fread w lbuffer\t\t"<<512*N*1.0/cput.split()<<" "<<512*N*1.0/wct.split()<<endl;
	  
	  // read
	  cput.reset();wct.reset();
	  for(int x = 0; x<10; ++x) tot += testread(name,N);
	  cout<<"read2 \t\t\t"<<512*N*1.0/cput.split()<<" "<<512*N*1.0/wct.split()<<endl;
	  
	  // mmap
	  cput.reset();wct.reset();
	  for(int x = 0; x<10; ++x) tot += testwmmap(name,N);
	  cout<<"mmap \t\t\t"<<512*N*1.0/cput.split()<<" "<<512*N*1.0/wct.split()<<endl;

	  cput.reset();wct.reset();
	  for(int x = 0; x<10; ++x) tot += testwithCpp(name,N);
	  cout<<"Cpp\t\t\t"<<512*N*1.0/cput.split()<<" "<<512*N*1.0/wct.split()<<endl;

  	  ::remove(name);
	}
	///

}
