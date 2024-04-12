#include <iostream>
#include "cpp-httplib/httplib.h"
#include "KontoEndpoints.h"

int main() {
    httplib::Server server;
    server.Get("/stop", [&](const httplib::Request&, httplib::Response&) {
        std::cout << "Shutting down server!" << std::endl;
        server.stop();
        });
    server.Get("/konten", std::function<void(const httplib::Request&, httplib::Response&)>(KontoHandler));
    std::cout << "Homeaccounting started!" << std::endl;
    server.listen("0.0.0.0", 8080);
    return 0;
}