
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

// designed to look like node pull request
std::string find_string_escape_node(std::string_view file_path) {
  // Avoid unnecessary allocations.
  size_t pos = file_path.empty() ? std::string_view::npos : file_path.find('%');
  if (pos == std::string_view::npos) {
    return std::string(file_path);
  }
  // Escape '%' characters to a temporary string.
  std::string escaped_file_path;
  do {
    escaped_file_path += file_path.substr(0, pos + 1);
    escaped_file_path += "25";
    file_path = file_path.substr(pos + 1);
    pos = file_path.empty() ? std::string_view::npos : file_path.find('%');
  } while (pos != std::string_view::npos);
  escaped_file_path += file_path;
  return escaped_file_path;
}



// designed to look like node pull request with character counting
std::string find_string_escape_node_count(std::string_view file_path) {
  // Avoid unnecessary allocations.
  size_t pos = file_path.empty() ? std::string_view::npos : file_path.find('%');
  if (pos == std::string_view::npos) {
    return std::string(file_path);
  }
  // Escape '%' characters to a temporary string.
  std::string escaped_file_path;
  size_t count = (size_t)std::count_if(file_path.begin(), file_path.end(),[]( char c ){ return c =='%'; });
  escaped_file_path.reserve(file_path.size() + count * 2);
  do {
    escaped_file_path += file_path.substr(0, pos + 1);
    escaped_file_path += "25";
    file_path = file_path.substr(pos + 1);
    pos = file_path.empty() ? std::string_view::npos : file_path.find('%');
  } while (pos != std::string_view::npos);
  escaped_file_path += file_path;
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
