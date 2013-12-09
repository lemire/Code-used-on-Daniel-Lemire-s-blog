/**
* Testing delta coding and decoding.
* D. Lemire, July 2012
*
* Best results with:
* 

$ g++-4.7  -funroll-loops -O3 -o unrolldeltasSIMD unrolldeltasSIMD.cpp

$ ./unrolldeltas 
*/


// This may require GCC 4.4/


#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iostream>
#include <cassert>
#include <vector>
#include <stdexcept>

#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <immintrin.h>

using namespace std;

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
    int elapsed() {
        return (t2.tv_sec * 1000 + t2.tv_usec / 1000) - (t1.tv_sec * 1000 + t1.tv_usec / 1000);
    }
    int split() {
        gettimeofday(&t2, 0);
        return elapsed();
    }
};




vector<int>  givemeanarray(size_t N) {
	vector<int> bigarray;
	bigarray.reserve(N);
	for(unsigned int k = 0; k<N; ++k)
	  bigarray.push_back(k+k/3);
	return bigarray;
}

enum{minoffset=0};
//#pragma GCC optimize("unroll-loops","O3") 
//#pragma GCC push_options

template <int mindist>
void delta(vector<int> & data) {
      if(data.size() == 0) return;
      for (size_t i = data.size() - 1; i > 0; --i) {
         data[i] -=  data[i - 1] + mindist;
      }
}

template <int mindist>
void deltaForSIMD(int* pData, size_t TotalQty) {
    if (TotalQty < 5) return;

    for (size_t i = TotalQty - 1; i >= 4; --i) {
        pData[i] -= pData[i-4] + mindist;
    }
};


template <int mindist>
void deltaSIMD(int* pData, size_t TotalQty) {
    if (TotalQty < 5) return;

    size_t      Qty4 = TotalQty / 4;

    if (Qty4 >= 2) {
        __v4si                  _u = {mindist, mindist, mindist, mindist};
        register __m128i        u = reinterpret_cast<__m128i>(_u);

        register __m128i*       pCurr = reinterpret_cast<__m128i*>(pData) + Qty4 - 1;
        register __m128i*       pStart = reinterpret_cast<__m128i*>(pData);
        register __m128i        a = _mm_loadu_si128(pCurr);

        while (pCurr > pStart) {
            register __m128i    b = _mm_loadu_si128(pCurr - 1);

            *pCurr-- = _mm_sub_epi32(a, _mm_add_epi32(b, u));
            a = b;
        }
        for (size_t i = TotalQty - 1; i >= 4 * Qty4; --i) {
            pData[i] -= pData[i-4] + mindist;
        }
    } else {
        for (size_t i = TotalQty - 1; i >= 4; --i) {
            pData[i] -= pData[i-4] + mindist;
        }
    }
}

template <int mindist>
void inverseDeltaSIMDUnrolled(int* pData, size_t TotalQty);

template <>
void inverseDeltaSIMDUnrolled<0>(int* pData, size_t TotalQty) {
    if (TotalQty < 5) return;

    size_t      Qty4 = TotalQty / 4;
    unsigned    UnrollQty = 4;

    if (Qty4 >= UnrollQty) {
        register __m128i*       pCurr = reinterpret_cast<__m128i*>(pData);
        register __m128i*       pEnd = pCurr + Qty4;
        register __m128i*       pEnd1 = pCurr + (Qty4 / UnrollQty) * UnrollQty;
        register __m128i        a, b;

        a = _mm_sub_epi32(a, a); // a <- 0

        while (pCurr < pEnd1) {
            b = _mm_loadu_si128(pCurr);
            *pCurr++ = a = _mm_add_epi32(a, b);
            
            b = _mm_loadu_si128(pCurr);
            *pCurr++ = a = _mm_add_epi32(a, b);
            
            b = _mm_loadu_si128(pCurr);
            *pCurr++ = a = _mm_add_epi32(a, b);
            
            b = _mm_loadu_si128(pCurr);
            *pCurr++ = a = _mm_add_epi32(a, b);
        }

        while (pCurr < pEnd) {
            b = _mm_loadu_si128(pCurr);
            *pCurr++ = a = _mm_add_epi32(a, b);
        }

        for (size_t i = Qty4 * 4; i < TotalQty; ++i) {
            pData[i] += pData[i-4];
        }
    } else {
        for (size_t i = 4; i < TotalQty; ++i) {
            pData[i] += pData[i-4];
        }
    }
}

template <>
void inverseDeltaSIMDUnrolled<1>(int* pData, size_t TotalQty) {
    if (TotalQty < 5) return;

    size_t      Qty4 = TotalQty / 4;
    unsigned    UnrollQty = 4;

    if (Qty4 >= UnrollQty) {
        register __m128i*       pCurr = reinterpret_cast<__m128i*>(pData);
        register __m128i*       pEnd = pCurr + Qty4;
        register __m128i*       pEnd1 = pCurr + (Qty4 / UnrollQty) * UnrollQty;
        register __m128i        a, b;
        __v4si                  _u = {1, 1, 1, 1};
        register __m128i        u = reinterpret_cast<__m128i>(_u);

        a = _mm_sub_epi32(a, a); // a <- 0
        a = _mm_sub_epi32(a, u); // a <- -1

        while (pCurr < pEnd1) {
            b = _mm_loadu_si128(pCurr);
            *pCurr++ = a = _mm_add_epi32(_mm_add_epi32(a, b), u);
            
            b = _mm_loadu_si128(pCurr);
            *pCurr++ = a = _mm_add_epi32(_mm_add_epi32(a, b), u);
            
            b = _mm_loadu_si128(pCurr);
            *pCurr++ = a = _mm_add_epi32(_mm_add_epi32(a, b), u);
            
            b = _mm_loadu_si128(pCurr);
            *pCurr++ = a = _mm_add_epi32(_mm_add_epi32(a, b), u);
        }

        while (pCurr < pEnd) {
            b = _mm_loadu_si128(pCurr);
            *pCurr++ = a = _mm_add_epi32(_mm_add_epi32(a, b), u);
        }

        for (size_t i = Qty4 * 4; i < TotalQty; ++i) {
            pData[i] += pData[i-4] + 1;
        }
    } else {
        for (size_t i = 4; i < TotalQty; ++i) {
            pData[i] += pData[i-4] + 1;
        }
    }
}


template <int mindist>
void inverseDeltaSIMD(int* pData, size_t TotalQty);

template <>
void inverseDeltaSIMD<0>(int* pData, size_t TotalQty) {
    if (TotalQty < 5) return;

    size_t Qty4 = TotalQty / 4;

    if (Qty4 >= 2) {
        register __m128i*       pCurr = reinterpret_cast<__m128i*>(pData);
        register __m128i*       pEnd = pCurr + Qty4;
        register __m128i        a = _mm_loadu_si128(pCurr++);

        while (pCurr < pEnd) {
            register __m128i        b = _mm_loadu_si128(pCurr);
            a = _mm_add_epi32(a, b);
            *pCurr++ = a;
        }

        for (size_t i = Qty4 * 4; i < TotalQty; ++i) {
            pData[i] += pData[i-4];
        }
    } else {
        for (size_t i = 4; i < TotalQty; ++i) {
            pData[i] += pData[i-4];
        }
    }
}


template <>
void inverseDeltaSIMD<1>(int* pData, size_t TotalQty) {
    if (TotalQty < 5) return;

    size_t Qty4 = TotalQty / 4;

    if (Qty4 >= 2) {
        register __m128i*       pCurr = reinterpret_cast<__m128i*>(pData);
        register __m128i*       pEnd = pCurr + Qty4;
        register __m128i        a = _mm_loadu_si128(pCurr++);
        __v4si                  _u = {1, 1, 1, 1};
        register __m128i        u = reinterpret_cast<__m128i>(_u);

        while (pCurr < pEnd) {
            register __m128i        b = _mm_loadu_si128(pCurr);
            *pCurr++ = a = _mm_add_epi32(_mm_add_epi32(a, b), u);
        }

        for (size_t i = Qty4 * 4; i < TotalQty; ++i) {
            pData[i] += pData[i-4] + 1;
        }
    } else {
        for (size_t i = 4; i < TotalQty; ++i) {
            pData[i] += pData[i-4] + 1;
        }
    }

}

template <int mindist>
void slowishinverseDelta1(vector<int> & data) {
      if(data.size() == 0) return;
      for (size_t i = 1; i != data.size(); ++i) {
         data[i] += data[i - 1] + mindist;
      }
}

template <int mindist>
void slowishinverseDelta2(vector<int> & data) {
      if(data.size() == 0) return;

       int a = data[0];
       for (size_t i = 1; i < data.size(); ++i)
           a = data[i] += a + mindist;
}

// loop unrolling helps avoid the need for -funroll-loops
template <int mindist>
void inverseDeltaMem(vector<int> & data) {
      if(data.size() == 0) return;
      size_t i = 1;

      int a = data[0];
      for (; i < data.size() - 1; i+=2) {
         a = data[i] += a + mindist;
         a = data[i+1] += a + mindist;
       }
      for (; i != data.size(); ++i) {
         data[i] += data[i - 1] + mindist;
      }
}



// loop unrolling helps avoid the need for -funroll-loops
template <int mindist>
void inverseDelta(vector<int> & data) {
      if(data.size() == 0) return;
      size_t i = 1;
      for (; i < data.size() - 1; i+=2) {
         data[i] += data[i - 1] + mindist;
         data[i+1] += data[i ] + mindist;
       }
      for (; i != data.size(); ++i) {
         data[i] += data[i - 1] + mindist;
      }
}

// loop unrolling helps avoid the need for -funroll-loops
template <int mindist>
void inverseDeltaMy1(vector<int> & data) {
#if 0
    if(data.size() == 0) return;

    const int UnrollQty = 4;
    size_t sz0 = (data.size() / UnrollQty)  * UnrollQty; // equal to 0, if data.size() < UnrollQty

    if (sz0>=UnrollQty) sz0 -= UnrollQty;

    size_t i = 1;

    int a = data[0];
    for (; i < sz0; i += UnrollQty) {
        a = data[i] += a + mindist;
        a = data[i + 1] += a + mindist;
        a = data[i + 2] += a + mindist;
        a = data[i + 3] += a + mindist;
    }

    for (; i != data.size(); ++i) {
       data[i] += data[i- 1] + mindist;
    }
#else
    if(data.size() == 0) return;

        const size_t UnrollQty = 4;
        size_t sz0 = (data.size() / UnrollQty)  * UnrollQty; // equal to 0, if data.size() < UnrollQty
        size_t i = 1;

        if (sz0>=UnrollQty) {
            int a = data[0];
            for (; i < sz0 - UnrollQty ; i += UnrollQty) {
                a = data[i] += a + mindist;
                a = data[i + 1] += a + mindist;
                a = data[i + 2] += a + mindist;
                a = data[i + 3] += a + mindist;
            }
        }
        
    for (; i != data.size(); ++i) {
        data[i] += data[i- 1] + mindist;
    }
#endif
}

#if 0
template <int mindist>
void inverseDeltaMy2(vector<int> & data) {
    if(data.size() == 0) return;

    const int UnrollQty = 16;
    size_t sz0 = (data.size() / UnrollQty)  * UnrollQty; // equal to 0, if data.size() < UnrollQty

    if (sz0>=UnrollQty) sz0 -= UnrollQty;

    int *pEnd1 = &data[sz0];
    int *pEnd2 = &data[data.size()];
    int *p2   = &data[1];
    int *p1   = &data[0];

    for (; p2 < pEnd1; p1 += UnrollQty, p2 += UnrollQty) {
        p2[0] += p1[1-1] + mindist;
        p2[1] += p1[1]   + mindist;
        p2[2] += p1[1+1] + mindist;
        p2[3] += p1[1+2] + mindist;
        p2[4] += p1[1+3] + mindist;
        p2[5] += p1[1+4] + mindist;
        p2[6] += p1[1+5] + mindist;
        p2[7] += p1[1+6] + mindist;

        p2[8] += p1[1+7] + mindist;
        p2[9] += p1[1+8] + mindist;
        p2[10] += p1[1+9] + mindist;
        p2[11] += p1[1+10] + mindist;
        p2[12] += p1[1+11] + mindist;
        p2[13] += p1[1+12] + mindist;
        p2[14] += p1[1+13] + mindist;
        p2[15] += p1[1+14] + mindist;
    }

    for (; p2 != pEnd2; ++p1, ++p2) {
       *p2 += *p1  + mindist;
    }
}
#endif

// loop unrolling helps avoid the need for -funroll-loops
template <int mindist>
void inverseDeltaMy3(vector<int> & data) {
    if(data.size() == 0) return;

    const int UnrollQty = 32;
    size_t sz0 = (data.size() / UnrollQty)  * UnrollQty; // equal to 0, if data.size() < UnrollQty

    if (sz0>=UnrollQty) sz0 -= UnrollQty;

    int *pEnd1 = &data[sz0];
    int *pEnd2 = &data[data.size()];
    int *p2   = &data[1];
    int *p1   = &data[0];

    for (; p2 < pEnd1; ) {
        *p2 += *p1 + mindist; ++p2; ++p1;
        *p2 += *p1 + mindist; ++p2; ++p1;
        *p2 += *p1 + mindist; ++p2; ++p1;
        *p2 += *p1 + mindist; ++p2; ++p1;

        *p2 += *p1 + mindist; ++p2; ++p1;
        *p2 += *p1 + mindist; ++p2; ++p1;
        *p2 += *p1 + mindist; ++p2; ++p1;
        *p2 += *p1 + mindist; ++p2; ++p1;

        *p2 += *p1 + mindist; ++p2; ++p1;
        *p2 += *p1 + mindist; ++p2; ++p1;
        *p2 += *p1 + mindist; ++p2; ++p1;
        *p2 += *p1 + mindist; ++p2; ++p1;

        *p2 += *p1 + mindist; ++p2; ++p1;
        *p2 += *p1 + mindist; ++p2; ++p1;
        *p2 += *p1 + mindist; ++p2; ++p1;
        *p2 += *p1 + mindist; ++p2; ++p1;
   }

    for (; p2 != pEnd2; ) {
        *p2 += *p1 + mindist; ++p2; ++p1;
    }
}

template <int mindist>
void test(size_t N ) {
    WallClockTimer time;
    for(int t = 0; t<2;++t) {
      cout <<" test # "<< t<<endl;
      vector<int> data = givemeanarray(N) ;


      {
          vector<int> copydata(data);

          cout << "min distance between ints is "<<mindist<<endl;

          time.reset();
          delta<mindist>(data);
          cout<<"delta speed "<<N/(1000.0*time.split())<<endl;
          time.reset();
          slowishinverseDelta1<mindist>(data);
          cout<<"Slowish(1) inverse delta speed "<<N/(1000.0*time.split())<<endl;
          if(data != copydata) throw runtime_error("bug!");
          cout<<endl;

      }

      {
          vector<int> copydata(data);

          cout << "min distance between ints is "<<mindist<<endl;

          time.reset();
          delta<mindist>(data);
          cout<<"delta speed "<<N/(1000.0*time.split())<<endl;
          time.reset();
          slowishinverseDelta2<mindist>(data);
          cout<<"Slowish(2) inverse delta speed "<<N/(1000.0*time.split())<<endl;
          if(data != copydata) throw runtime_error("bug!");
          cout<<endl;

      }

      {
          vector<int> copydata(data);

          cout << "min distance between ints is "<<mindist<<endl;

          time.reset();
          delta<mindist>(data);
          cout<<"delta speed "<<N/(1000.0*time.split())<<endl;
          time.reset();
          inverseDelta<mindist>(data);
          cout<<"Unroll2 inverse delta speed "<<N/(1000.0*time.split())<<endl;
          if(data != copydata) throw runtime_error("bug!");
          cout<<endl;

      }

      {
          vector<int> copydata(data);

          cout << "min distance between ints is "<<mindist<<endl;

          time.reset();
          delta<mindist>(data);
          cout<<"delta speed "<<N/(1000.0*time.split())<<endl;
          time.reset();
          inverseDeltaMem<mindist>(data);
          cout<<"Unroll2 (mem) inverse delta speed "<<N/(1000.0*time.split())<<endl;
          if(data != copydata) throw runtime_error("bug!");
          cout<<endl;

      }




      {
          vector<int> copydata(data);

          cout << "min distance between ints is "<<mindist<<endl;

          time.reset();
          delta<mindist>(data);
          cout<<"delta speed "<<N/(1000.0*time.split())<<endl;
          time.reset();
          inverseDeltaMy1<mindist>(data);
          cout<<"My1 inverse delta speed "<<N/(1000.0*time.split())<<endl;
          if(data != copydata) throw runtime_error("bug!");
          cout<<endl;

      }

#if 0
      {
          vector<int> copydata(data);

          cout << "min distance between ints is "<<mindist<<endl;

          time.reset();
          delta<mindist>(data);
          cout<<"delta speed "<<N/(1000.0*time.split())<<endl;
          time.reset();
          inverseDeltaMy2<mindist>(data);
          cout<<"My2 inverse delta speed "<<N/(1000.0*time.split())<<endl;
          if(data != copydata) throw runtime_error("bug!");
          cout<<endl;
      }


      {
          vector<int> copydata(data);

          cout << "min distance between ints is "<<mindist<<endl;

          time.reset();
          delta<mindist>(data);
          cout<<"delta speed "<<N/(1000.0*time.split())<<endl;
          time.reset();
          inverseDeltaMy3<mindist>(data);
          cout<<"My3 inverse delta speed "<<N/(1000.0*time.split())<<endl;
          if(data != copydata) throw runtime_error("bug!");
          cout<<endl;
      }
#endif


      {
          int* pCopyData = (int*)memalign(16, data.size() * sizeof data[0]);
          if (!pCopyData) {
            throw runtime_error("Not enough memory");
          }
          memcpy(pCopyData, &data[0], data.size() * sizeof data[0]);

          cout << "min distance between ints is "<<mindist<<endl;

          time.reset();
          deltaForSIMD<mindist>(pCopyData, data.size());
          cout<<"for SIMD delta speed "<<N/(1000.0*time.split())<<endl;
          time.reset();
          inverseDeltaSIMD<mindist>(pCopyData, data.size());
          cout<<"SIMD inverse delta speed "<<N/(1000.0*time.split())<<endl;
          for (size_t i = 0; i < data.size(); ++i) {
            if (data[i] != pCopyData[i]) {
                cerr << "Elem index: " << i << " orig: " << data[i] << " obtained: " << pCopyData[i] << endl;
                throw runtime_error("bug");
             }
          }
          free(pCopyData);
          cout<<endl;
      }

      {
          int* pCopyData = (int*)memalign(16, data.size() * sizeof data[0]);
          if (!pCopyData) {
            throw runtime_error("Not enough memory");
          }
          memcpy(pCopyData, &data[0], data.size() * sizeof data[0]);

          cout << "min distance between ints is "<<mindist<<endl;

          time.reset();
          deltaForSIMD<mindist>(pCopyData, data.size());
          cout<<"for SIMD delta speed "<<N/(1000.0*time.split())<<endl;
          time.reset();
          inverseDeltaSIMDUnrolled<mindist>(pCopyData, data.size());
          cout<<"SIMD inverse UNROLLED delta speed "<<N/(1000.0*time.split())<<endl;
          for (size_t i = 0; i < data.size(); ++i) {
            if (data[i] != pCopyData[i]) {
                cerr << "Elem index: " << i << " orig: " << data[i] << " obtained: " << pCopyData[i] << endl;
                throw runtime_error("bug");
             }
          }
          free(pCopyData);
          cout<<endl;
      }


      cout<<endl<<endl<<endl;
    }
}

      


int main() {
	cout << "reporting speed in million of integers per second "<<endl;
/*
    size_t aLengths[] = {1, 2, 3, 4, 
                       5, 6, 7, 8, 
                       9, 10, 11, 12, 
                       13, 14, 15, 16,
                       17, 18, 19, 20,
                       21, 22, 23, 24,
                       25, 26, 27, 28,
                       29, 30, 31, 32,
                       50 * 1000 * 100,
                       50 * 1000 * 1000};
                       */
    size_t aLengths[] = { 50 * 1000 * 100};
    size_t LenQty = sizeof(aLengths) / sizeof aLengths[0];

    for (size_t i = 0; i < LenQty; ++i) {
        size_t N = aLengths[i];
        try {
            test<1>(N);
        } catch (...) {
            cerr << "MINDIST: 1 Failed for N = " << N << endl;
            throw;
        }
    }

    cout<<"============"<<endl;

    for (size_t i = 0; i < LenQty; ++i) {
        size_t N = aLengths[i];
        try {
            test<0>(N);
        } catch (...) {
            cerr << "MINDIST: 0 Failed for N = " << N << endl;
            throw;
        }
    }

    cout << LenQty << endl;

}

//#pragma GCC pop_options
