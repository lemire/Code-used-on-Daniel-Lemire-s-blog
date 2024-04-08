#include "crow.h"
#include <cstdlib>
int main(int argc, char **argv) {
  int thread_count = 0;
  if (argc > 0) {
    thread_count = atoi(argv[1]);
  }
  crow::SimpleApp app;
  app.loglevel(crow::LogLevel::Warning);

  CROW_ROUTE(app, "/simple")([]() { return "Hello world"; });
  if (thread_count) {
    printf("using %d threads\n", thread_count);
    app.port(18080).concurrency(thread_count).run();
  } else {
    app.port(18080).multithreaded().run();
  }
}
