#pragma once
namespace Response {
	struct Response {
		/**
		* HTTP Status codes, e.g. 404, 200, 500
		*/
		const unsigned short status;
		/**
		* e.g. JSON representation of a database entry or several database entries.
		*/
		const char* payload;
	};
}