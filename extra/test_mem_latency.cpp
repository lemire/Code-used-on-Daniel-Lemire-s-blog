
// stolen from Leonid
//g++ -std=c++11 -O3 -o test_mem_latency test_mem_latency.cpp 
// g++ -DuseHugePage=1 -std=c++11 -O3 -o test_mem_latency test_mem_latency.cpp 
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>

#include <string.h>
#include <sys/mman.h>

#include "ztimer.h"

using namespace std;

void Test(int SeqScenario, char *mem, size_t MemSize, size_t Qty, size_t Gap) {
  size_t TotalQty = Qty;


  WallClockTimer wtm;
  CPUTimer       ctm;

  wtm.reset();
  ctm.reset();

  size_t sum = 0;

  string accessType;

  if (SeqScenario == 0) {
    accessType = "sequential";
    TotalQty = min(MemSize, Qty);
    for (size_t i = 0; i < TotalQty; ++i) {
      sum += mem[i];
    }
  } else if (SeqScenario == 2) {
    accessType = "random";
    for (size_t i = 0; i < TotalQty; ++i) {
      sum += mem[rand() % MemSize];
    }
  } else {
    stringstream s;
    s << "sequential gapped, gap: " << Gap << " bytes" ;
    accessType = s.str();
    for (size_t i = 0, pos = 0; i < TotalQty; ++i) {
      sum += mem[pos];
      pos += Gap; 
      if (pos > MemSize) pos %= MemSize;
    }
  }


  wtm.split();
  ctm.split();

  cout << "Access type: " << accessType << endl;
  cout << "Wall elapsed: " << wtm.elapsed() / 1e6 << endl;
  cout << "CPU elapsed: " << ctm.elapsed() / 1e6 << endl;
  cout << "One access: " << double(ctm.elapsed() / double(TotalQty)) << " microsecs" << endl;
  cout << "Read speed: " << (1e6* TotalQty /  (1024.0*1024.0*1024.0)/ wtm.elapsed()) << " GBs/sec " << endl;
  cout << "Ignore: " << sum << " Qty: " << TotalQty  << endl;
  cout << "================================================================================" << endl;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cerr << "Usage: <amount of mem in gigs (fractional is fine)> <use huge page>" << endl;
    return 1;
  }
  const size_t Qty = 256UL * 1024UL * 1024UL;

  size_t MemSize = static_cast<size_t>(round(atof(argv[1]) * 1024.0 * 1024.0 * 1024.0));

  bool useHugePage = atol(argv[2]);

  cout << "Mem size: " << MemSize << " bytes" << endl;
  cout << "Huge page: " << useHugePage << endl;

  cout << "Allocating memory: " << MemSize << endl;
  
  char* mem = NULL;

  if (useHugePage) { // As suggested by Nathan Kurz
    mem = reinterpret_cast<char *>(mmap(NULL, MemSize, PROT_READ | PROT_WRITE,
                                        MAP_PRIVATE| MAP_ANONYMOUS, -1, 0));
    madvise(mem, MemSize, MADV_HUGEPAGE);
  } else {
    mem = new char[MemSize];
  }

  for (size_t i = 0; i + 1024 < MemSize; i += 1024)
    memset(mem + i, rand() % 256, 1024);

  Test(0, mem, MemSize, Qty, 1024*127);
  Test(1, mem, MemSize, Qty/4, 1024*127);
  Test(2, mem, MemSize, Qty/16, 1024*127);

  return 0;

}
