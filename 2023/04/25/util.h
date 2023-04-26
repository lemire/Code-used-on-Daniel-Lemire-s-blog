

void print_bitmask(bitmask &b, std::string_view view) {
  std::string bit_line;
  std::string str_line;

  for (size_t i = 0; i < view.size(); i++) {

    if (view[i] == '\n') {
      str_line += "\\n";
      bit_line += (b.comment[i / 64] & (uint64_t(1) << (i % 64)) ? " 1" : " 0");
      std::cout << str_line << std::endl;
      std::cout << bit_line << std::endl;
      std::cout << "-----------------------" << std::endl;
      bit_line.clear();
      str_line.clear();
    } else {
      str_line += view[i];
      bit_line += b.comment[i / 64] & (uint64_t(1) << (i % 64)) ? "1" : "0";
    }
  }
  if (!str_line.empty()) {
    std::cout << str_line << std::endl;
    std::cout << bit_line << std::endl;
    std::cout << "-----------------------" << std::endl;
  }
}