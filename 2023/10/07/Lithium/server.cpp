#include <lithium_http_server.hh>
            
int main() {
  li::http_api my_api;
              
  my_api.get("/simple") = 
  [&](li::http_request& request, li::http_response& response) {
    response.write("hello world.");
  };
  li::http_serve(my_api, 3000);
}