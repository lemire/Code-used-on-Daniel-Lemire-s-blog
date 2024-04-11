#include <algorithm>
#include <array>
#include <initializer_list>
#include <ranges>
#include <set>

struct vec : std::array<int, 2> {
  vec(int x, int y) : std::array<int, 2>{x, y} {}
  vec operator+(const vec &v) { return {at(0) + v[0], at(1) + v[1]}; }
};

auto ortho_neighbors =
    std::initializer_list<vec>{{1, 0}, {0, 1}, 
      {0, -1}, {-1, 0}};

auto neighbors(const vec& v) {
  return std::views::transform(ortho_neighbors, 
    [&v](auto x) {
      return x + v; });
}

std::set<vec> bigset;

int main() {
  vec v(3, 4);
     // printf("%d %d\n", v[0], v[1]);

  size_t count = 0;
  for (vec adj : neighbors(v)) {
    printf("%d %d\n", adj[0], adj[1]);
    if (bigset.contains(adj)) {
      count++;
    }
  }
  return count;
}
