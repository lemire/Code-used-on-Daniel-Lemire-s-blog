
#include "escape.h"
#include <sstream>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

std::string string_escape(const std::string_view file_path) {
  std::string escaped_file_path;
  for (size_t i = 0; i < file_path.length(); ++i) {
    escaped_file_path += file_path[i];
    if (file_path[i] == '%')
      escaped_file_path += "25";
  }
  return escaped_file_path;
}

std::string find_string_escape(std::string_view str) {
  std::string escaped_file_path;
  size_t pos = 0;
  while ((pos = str.find('&', pos)) != std::string_view::npos) {
    escaped_file_path += str.substr(0, pos + 1);
    escaped_file_path += "25";
    str = str.substr(pos + 1);
    pos = 0;
  }
  escaped_file_path += str;
  return escaped_file_path;
}

std::string stream_escape(const std::string_view file_path) {
  std::ostringstream escaped_file_path;
  for (size_t i = 0; i < file_path.length(); ++i) {
    escaped_file_path << file_path[i];
    if (file_path[i] == '%')
      escaped_file_path << "25";
  }
  return escaped_file_path.str();
}
