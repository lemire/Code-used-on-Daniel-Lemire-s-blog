#include "httplib.h"
using namespace httplib;
#include <format>
int main() {

  Server svr;

  svr.Post("/simple", [](const Request& req, Response& res) {
    auto url = req.body;
    res.set_content(std::format(R"({"parsed":"{}"})",req.body), "application/json");
  });

  // Match the request path against a regular expression
  // and extract its captures
  svr.Get(R"(/numbers/(\d+))", [&](const Request& req, Response& res) {
    auto numbers = req.matches[1];
    res.set_content(numbers, "text/plain");
  });

  // Capture the second segment of the request path as "id" path param
  svr.Get("/users/:id", [&](const Request& req, Response& res) {
    auto user_id = req.path_params.at("id");
    res.set_content(user_id, "text/plain");
  });

  // Extract values from HTTP headers and URL query params
  svr.Get("/body-header-param", [](const Request& req, Response& res) {
    if (req.has_header("Content-Length")) {
      auto val = req.get_header_value("Content-Length");
    }
    if (req.has_param("key")) {
      auto val = req.get_param_value("key");
    }
    res.set_content(req.body, "text/plain");
  });

  svr.Get("/stop", [&](const Request& req, Response& res) {
    svr.stop();
  });

  svr.listen("localhost", 1234);
}