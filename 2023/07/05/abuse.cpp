#include <regex>

struct convenience_matcher {
  convenience_matcher(const char *str) : re(str) {}
  bool match(const std::string &s) {
    std::smatch base_match;
    return std::regex_match(s, base_match, re);
  }
  bool operator()(const std::string &s) { return match(s); }
  std::regex re;
};

inline convenience_matcher operator"" _re(const char *str, size_t len) {
  return convenience_matcher(str);
}

bool match(std::string s) { return "\\d+"_re(s); }

bool match() { return "\\d+"_re("123"); }

int main() {
  printf("%d \n", "\\d+"_re("123"));
  printf("%d \n", "\\d+"_re("a23"));
  printf("%d \n", R"(\d+)"_re("123"));

  printf("%d \n", R"(\d+)"_re("a23"));
}
