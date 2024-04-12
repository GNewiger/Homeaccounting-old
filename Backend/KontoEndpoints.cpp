#include "KontoEndpoints.h"
#include "cpp-httplib/httplib.h";

void KontoHandler(const httplib::Request& req, httplib::Response& res) {
    res.set_content("<head><body>Hello from Homeaccounting!</body></head>", "text/html");
    return;
};