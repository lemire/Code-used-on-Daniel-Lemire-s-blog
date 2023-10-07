#define CPPHTTPLIB_USE_POLL
#define CPPHTTPLIB_THREAD_POOL_COUNT 64
#include "httplib.h"
#include  <fmt/core.h>
#include  <iostream>
#include "simdjson.h"
#include "ada.h"

using namespace httplib;
using namespace simdjson;

int main() {

  Server svr;
  /*svr.set_logger([](const auto& req, const auto& res) {
    std::cout << req.method << std::endl;
    std::cout << req.body << std::endl;
  });*/

svr.Get("/simple", [](const Request& req, Response& res) {
  res.set_content(R"({{"parsedresult":null}})", "application/json");
      return;
  });
  svr.Post("/simple", [](const Request& req, Response& res) {

   /* padded_string url(req.body);

    ondemand::parser parser; // should have just one per thread
    ondemand::document doc = parser.iterate(url);
    std::string_view url_string;
    if(doc["url"].get_string().get(url_string)) {*/
      res.set_content(R"({{"parsedresult":null}})", "application/json");
      return;
    //}
    // next line is just a prototype
   // res.set_content(fmt::format(R"({{"parsedresult":"{}"}})", url_string), "application/json");
  });

  svr.Post("/href", [](const Request& req, Response& res) {
    padded_string url(req.body);
    ondemand::parser parser; // should have just one per thread
    ondemand::document doc = parser.iterate(url);
    std::string_view url_string;
    if(doc["url"].get_string().get(url_string)) {
      res.set_content(R"({{"parsedresult":null\}})", "application/json");
      return;
    }

    auto urlstruct = ada::parse(url_string);
    if(!urlstruct) {
      res.set_content(R"({{"parsedresult":null}})", "application/json");
      return;   
    }
    // next line is just a prototype
    res.set_content(fmt::format(R"({{"parsedresult":"{}"}})", urlstruct->get_href()), "application/json");
  });

  printf("localhost:3000\n");
  svr.listen("127.0.0.1", 3000);

  return EXIT_SUCCESS;
}