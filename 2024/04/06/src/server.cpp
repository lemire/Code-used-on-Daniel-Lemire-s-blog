#include "crow.h"

int main()
{
    crow::SimpleApp app;
    app.loglevel(crow::LogLevel::Warning);

    CROW_ROUTE(app, "/simple")([](){
        return "Hello world";
    });

    app.port(18080).multithreaded().run();
}