//  clang++  -std=c++11 -o stlsizeof stlsizeof.cpp
#include <cstdint>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <queue>
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <list>
#include <queue>
#include <map>
#include <vector>


uint64_t memory_usage;

// use this when calling STL object if you want
// to keep track of memory usage
template <class T> class MemoryCountingAllocator {
public:
    // type definitions
    typedef T value_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T &reference;
    typedef const T &const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;


    // rebind allocator to type U
    template <class U> struct rebind {
        typedef MemoryCountingAllocator<U> other;
    };

    pointer address(reference value) const {
        return &value;
    }
    const_pointer address(const_reference value) const {
        return &value;
    }

    MemoryCountingAllocator() : base() {}
    MemoryCountingAllocator(const MemoryCountingAllocator &) : base() {}
    template <typename U>
    MemoryCountingAllocator(const MemoryCountingAllocator<U> &) : base() {}
    ~MemoryCountingAllocator() {}

    // return maximum number of elements that can be allocated
    size_type max_size() const throw() {
        return base.max_size();
    }

    pointer allocate(size_type num, const void * p = 0) {
        memory_usage += num * sizeof(T);
        return base.allocate(num,p);
    }

    void construct(pointer p, const T &value) {
        return base.construct(p,value);
    }

    // destroy elements of initialized storage p
    void destroy(pointer p) {
        base.destroy(p);
    }

    // deallocate storage p of deleted elements
    void deallocate(pointer p, size_type num ) {
        memory_usage -= num * sizeof(T);
        base.deallocate(p,num);
    }
    std::allocator<T> base;
};

// for our purposes, we don't want to distinguish between allocators.
template <class T1, class T2>
bool operator==(const MemoryCountingAllocator<T1> &, const T2 &) throw() {
    return true;
}

template <class T1, class T2>
bool operator!=(const MemoryCountingAllocator<T1> &, const T2 &) throw() {
    return false;
}

void initializeMemUsageCounter()  {
    memory_usage = 0;
}

uint64_t getMemUsageInBytes()  {
    return memory_usage;
}


typedef std::set<uint32_t,std::less<uint32_t>,MemoryCountingAllocator<uint32_t> >  treeset;
typedef std::unordered_set<uint32_t,std::hash<uint32_t>,std::equal_to<uint32_t>,MemoryCountingAllocator<uint32_t> >  hashset;
typedef std::vector<uint32_t,MemoryCountingAllocator<uint32_t> >  vector;
typedef std::list<uint32_t,MemoryCountingAllocator<uint32_t> >  list;
typedef std::deque<uint32_t,MemoryCountingAllocator<uint32_t> >  deque;

int main() {
  size_t N = 1024;
  std::cout << "Displaying memory usage in bytes."<<std::endl;
  std::cout << "Filling data structures with " << N << " elements and reporting the per element memory usage."<<std::endl;
  initializeMemUsageCounter();
  assert(getMemUsageInBytes() == 0);

  vector v;
  for(uint32_t k = 0; k < N; k++) v.push_back(k);
  std::cout << "memory usage per element of a vector<uint32_t> : " << getMemUsageInBytes() * 1.0 / N << std::endl;

  initializeMemUsageCounter();
  assert(getMemUsageInBytes() == 0);

  list l;
  for(uint32_t k = 0; k < N; k++) l.push_back(k);
  std::cout << "memory usage per element of a list<uint32_t> : " << getMemUsageInBytes() * 1.0 / N << std::endl;

  initializeMemUsageCounter();
  assert(getMemUsageInBytes() == 0);


  deque dq;
  for(uint32_t k = 0; k < N; k++) dq.push_back(k);
  std::cout << "memory usage per element of a deque<uint32_t> : " << getMemUsageInBytes() * 1.0 / N << std::endl;


  initializeMemUsageCounter();
  assert(getMemUsageInBytes() == 0);
  hashset h;
  for(uint32_t k = 0; k < N; k++) h.insert(k);
  std::cout << "memory usage per element of an unordered_set<uint32_t> : " << getMemUsageInBytes() * 1.0 / N << std::endl;

  initializeMemUsageCounter();
  assert(getMemUsageInBytes() == 0);
  treeset t;
  for(uint32_t k = 0; k < N; k++) t.insert(k);
  std::cout << "memory usage per element of a set<uint32_t> : " << getMemUsageInBytes() * 1.0 / N << std::endl;

}
