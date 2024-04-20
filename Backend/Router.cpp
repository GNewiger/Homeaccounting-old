#include "Page.h"
#include "KontoEndpoints.h"
#include <string.h>
#include "Response.h"

namespace Router {
	const Page::Page pages[Konto::numberOfEndpoints] = {Konto::pages[0]};
	const Response::Response route(const char* url, const char* method) {
		for (const Page::Page& candidatePage : pages) {
			if (strcmp(candidatePage.url, url) == 0 && strcmp(candidatePage.method, method) == 0) {
				return candidatePage.handle();
			}
		}
		return { 404, "{}"};
	}
}
