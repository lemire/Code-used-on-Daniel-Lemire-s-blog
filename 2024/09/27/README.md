

rm -r -f build17 && CMAKE_CXX_STANDARD=17 cmake -B build17 -D CPM_SOURCE_CACHE=.cache && time cmake --build build17

3.98s
4.08s
4.17s

rm -r -f build20 && CMAKE_CXX_STANDARD=20 cmake -B build20 -D CPM_SOURCE_CACHE=.cache  && time cmake --build build20

4.30s
4.39s
4.35s