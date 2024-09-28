#include <simdjson.h>
#include <fast_float/fast_float.h>
#include <fmt/format.h>

int main(int argc, char *argv[]) {
    if(argc != 2) { std::cerr << "Usage: " << argv[0] << " <json-file>\n"; return EXIT_FAILURE; }
    simdjson::ondemand::parser parser;
    simdjson::padded_string json = simdjson::padded_string::load(argv[1]);
    simdjson::ondemand::document doc = parser.iterate(json);
    std::string_view url_string = doc["user"]["wealth"].get_string();
    double result;
    auto answer = fast_float::from_chars(url_string.data(), url_string.data()+url_string.size(), result);
    if(answer.ec != std::errc()) { std::cerr << "parsing failure\n"; return EXIT_FAILURE; }
    fmt::print("parsed the wealth {:.2f}\n", result);
    return EXIT_SUCCESS;
}