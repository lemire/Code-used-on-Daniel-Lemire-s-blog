#include "http.h" /* the HTTP facil.io extension */

// We'll use this callback in `http_listen`, to handles HTTP requests
void on_request(http_s *request);

// These will contain pre-allocated values that we will use often
FIOBJ HTTP_X_DATA;

// Listen to HTTP requests and start facil.io
int main(int argc, char const **argv) {
  // allocating values we use often
  HTTP_X_DATA = fiobj_str_new("X-Data", 6);
  // listen on port 3000 and any available network binding (NULL == 0.0.0.0)
  http_listen("3000", NULL, .on_request = on_request, .log = 0);
  // start the server
  fio_start();
  // deallocating the common values
  fiobj_free(HTTP_X_DATA);
}

// Easy HTTP handling
void on_request(http_s *request) {
  http_set_cookie(request, .name = "my_cookie", .name_len = 9, .value = "data",
                  .value_len = 4);
  http_set_header(request, HTTP_HEADER_CONTENT_TYPE,
                  http_mimetype_find((char*)"txt", 3));
  http_set_header(request, HTTP_X_DATA, fiobj_str_new("my data", 7));
  http_send_body(request, (void *)"Hello World!\r\n", 14);
}