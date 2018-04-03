#include <cpprest/http_client.h>
#include <stdexcept>

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;


constexpr auto HTTP_SUCCESS = 200;

int main() {
	const auto host = "http://localhost:7890";

	http_client_config config;
	web::credentials cred("user", "user");
	config.set_credentials(cred);
	
	try {
		http_client client(host, config);
		http_request request(methods::POST);
		uri_builder builder("api/test");
		request.set_request_uri(builder.to_string());

		request.set_body(R"({ "hello" : "world" })");

		auto job = client.request(request).then([](http_response response) {
			if (response.status_code() != HTTP_SUCCESS) {
				const auto errorString = std::string("HTTP Error: ") +
				                         std::to_string(response.status_code());
				throw std::logic_error(errorString);
			}
		});

		job.wait();
	} catch (const std::exception& e) {
		throw std::logic_error(e.what());
	} catch (...) {
		throw std::logic_error("Error: unknown exception");
	}
}

