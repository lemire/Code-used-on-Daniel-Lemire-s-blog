// D. Lemire: compile with g++ -O3 -std=c++11 -o vector-vs-array vector-vs-array.cpp 

// vector-vs-array.cpp
// Copyright 2013 Skand Hurkat

// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
// 
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along
// with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <vector>
#include <cstdlib>
#include <array>
#include <chrono>
#include <ratio>
#include <ctime>

using namespace std;

int main(void)
{
  // Begin C test. Fill declare array, fill it up, and repeat 10 times to
  // get average time taken to fill up 1000000 integers
  chrono::steady_clock::time_point start, end;
  int bogus = 0;
  int* a;
  int N = 1000000;
  int T = 1000;
  chrono::duration<double> time_span_cumulative = 
    chrono::duration<double>::zero();
  for(int i = 0; i < T; i++)
  {
    start = chrono::steady_clock::now();
    a = new int[N];
    if(a == nullptr)
    {
      cerr << "Fatal error with memory allocation" << endl;
      return -1;
    }
    for(int j = 0; j < N; j++)
    {
      a[j] = j;
    }
    bogus += a[100001];
    end = chrono::steady_clock::now();
    time_span_cumulative += 
      chrono::duration_cast<chrono::duration<double>>(end-start);
    delete[] a;
  }
  cout << "The C like implementation took "
       << time_span_cumulative.count()/1000 << " seconds" << endl;

  // Begin the C++ STL implementation
  time_span_cumulative = chrono::duration<double>::zero();
  vector<int> v;
  for(int i = 0; i < T; i++)
  {
    start = chrono::steady_clock::now();
    v.reserve(N);
    for(int j = 0; j < N; j++)
    {
      v.push_back(j);

    }
    bogus += v[100001];
    end = chrono::steady_clock::now();
    time_span_cumulative += 
      chrono::duration_cast<chrono::duration<double>>(end-start);
    vector<int>().swap(v);
  }
  cout << "The STL push_back implementation took "
       << time_span_cumulative.count()/1000 << " seconds" << endl;
  time_span_cumulative = chrono::duration<double>::zero();
  for(int i = 0; i < T; i++)
  {
    start = chrono::steady_clock::now();
    v.resize(N);
    for(int j = 0; j < N; j++)
    {
      v[j] = j;
    }
    bogus += v[100001];
     end = chrono::steady_clock::now();
    time_span_cumulative += 
      chrono::duration_cast<chrono::duration<double>>(end-start);
    vector<int>().swap(v);
  }
  cout << "The STL bracket implementation took "
       << time_span_cumulative.count()/1000 << " seconds" << endl;
   time_span_cumulative = chrono::duration<double>::zero();
  for(int i = 0; i < T; i++)
  {
    start = chrono::steady_clock::now();
    v.reserve(N);
    while(v.size()<N)
    {
      v.emplace_back(v.size());
    }
    bogus += v[100001];
     end = chrono::steady_clock::now();
    time_span_cumulative += 
      chrono::duration_cast<chrono::duration<double>>(end-start);
    vector<int>().swap(v);
  }
  cout << "The STL emplace_back implementation took "
       << time_span_cumulative.count()/1000 << " seconds" << endl;
   return bogus;
}
