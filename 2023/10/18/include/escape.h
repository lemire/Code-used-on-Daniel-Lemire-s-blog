#ifndef ESCAPE_H
#define ESCAPE_H

#include <string>
#include <string_view>
std::string string_escape(const std::string_view file_path);
std::string find_string_escape_node(std::string_view file_path);
std::string find_string_escape(const std::string_view file_path);
std::string stream_escape(const std::string_view file_path);
#endif // ESCAPE_H
