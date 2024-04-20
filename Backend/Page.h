#pragma once
#include "Response.h"
namespace Page {
	struct Page
	{
		/**
		 * Acceptable URL for this page.
		 */
		const char* url;

		/**
		* Request method ("GET", "POST", ...)
		*/
		const char* method;

		/**
		* Will be called when a request to the specified URL with the specified request method comes in. Gives the HTML response.
		*/
		const Response::Response (*handle)();
	};
}