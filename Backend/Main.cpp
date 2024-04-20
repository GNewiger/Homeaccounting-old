#include <iostream>
#include "libmicrohttpd/microhttpd.h"
#include "KontoEndpoints.h"
#include "Router.h"
#include "Page.h"
#include "Response.h"
#include <sys/types.h>
#include <winsock2.h>

#define PORT 8080

MHD_Result answer_to_connection(void* cls, struct MHD_Connection* connection,
    const char* url,
    const char* method, const char* version,
    const char* upload_data,
    size_t* upload_data_size, void** con_cls)
{
    const Response::Response& response = Router::route(url, method);
    struct MHD_Response* MHD_response;
    MHD_Result ret;

    MHD_response = MHD_create_response_from_buffer(strlen(response.payload),
        (void*)response.payload, MHD_RESPMEM_PERSISTENT);
    MHD_add_response_header(MHD_response, "Content-Type", "application/json");
    ret = MHD_queue_response(connection, response.status, MHD_response);
    MHD_destroy_response(MHD_response);

    return ret;
}

int main() {
	struct MHD_Daemon* daemon;

	daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL,
		answer_to_connection, NULL, MHD_OPTION_END);
	if (NULL == daemon) return 1;
	std::cout << "Homeaccounting Server started!" << std::endl;
	getchar();

	MHD_stop_daemon(daemon);
	return 0;
}