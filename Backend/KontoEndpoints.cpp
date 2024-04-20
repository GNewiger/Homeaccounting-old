#include "KontoEndpoints.h"
#include "Page.h"

namespace Konto {
	static const Response::Response allKonten() {
		return { 200, "{\"0\": {\"name\": \"Bafoeg\", \"isHaben\": 1}}" };
	}

	const Page::Page pages[numberOfEndpoints] = {
		{"/konten", "GET", allKonten}
	};
}