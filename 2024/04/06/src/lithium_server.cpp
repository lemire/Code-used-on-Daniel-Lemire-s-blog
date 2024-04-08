#include <lithium_http_server.hh>

int main(int argc, char **argv) {
  int thread_count = 0;
  if (argc > 0) {
    thread_count = atoi(argv[1]);
  }
  li::http_api my_api;

  my_api.get("/simple") = [&](li::http_request &request,
                              li::http_response &response) {
    response.write("hello world.");
  };
  if (thread_count) {
    printf("using %d threads\n", thread_count);
    li::http_serve(my_api, 3000, s::nthreads = thread_count);
  } else {
    li::http_serve(my_api, 3000);
  }
}
