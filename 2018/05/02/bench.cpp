#include <iostream>
#include <sstream>
#include <fstream>
#include "benchmark.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h" // you have to check in the submodule
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"


using namespace rapidjson;
using namespace std;


static pair<char *, size_t> get_corpus(string filename) {
    ifstream is(filename, ios::binary);
    if (is) {
        stringstream buffer;
        buffer << is.rdbuf();
        size_t length = buffer.str().size();
        char * aligned_buffer;
        if (posix_memalign( (void **)&aligned_buffer, 64, (length + 63) / 64  * 64)) {
            throw "Allocation failed";
        };
        memset(aligned_buffer, 0x20, (length + 63) / 64  * 64);
        memcpy(aligned_buffer, buffer.str().c_str(), length);
        is.close();
        return make_pair((char *)aligned_buffer, length);
    }
    throw "No corpus";
    return make_pair((char *)0, (size_t)0);
}
std::string rapidstringme(char * json) {
    Document d;
    d.Parse(json);
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
    return buffer.GetString();
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0] << " <jsonfile>\n";
    cerr << "Or " << argv[0] << " -v <jsonfile>\n";
    exit(1);
  }
  bool verbose = false;
  if (argc > 2) {
    if (strcmp(argv[1], "-v"))
      verbose = true;
  }
  pair<char *, size_t> p = get_corpus(argv[argc - 1]);
  size_t volume = p.second;
  int repeat = 10;

  rapidjson::Document d;
  char * buffer = (char *) malloc(p.second);
  memcpy(buffer, p.first, p.second);
  BEST_TIME(d.Parse((const char *)buffer).HasParseError(), false,
            memcpy(buffer, p.first, p.second), repeat, volume, true);
  BEST_TIME(d.ParseInsitu(buffer).HasParseError(), false,
            memcpy(buffer, p.first, p.second), repeat, volume, true);
  size_t strlength = rapidstringme((char*) p.first).size();
  if(verbose) std::cout << "input length is "<< p.second << " stringified length is " << strlength << std::endl;
  BEST_TIME_NOCHECK(rapidstringme((char*) p.first), , repeat, volume,
                    true);
  free(buffer);
}
