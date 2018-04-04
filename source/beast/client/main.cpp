#include <string>
#include <iostream>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;

int main() {
	const std::string host = "localhost";
	const std::string port = "9876";
	const std::string uri = "/api/";

	boost::asio::io_context io_context;

	// performing I/O
	tcp::resolver resolver{io_context};
	tcp::socket socket{io_context};

	// look up domain name
	auto const res = resolver.resolve(host, port);

	// make connection
  boost::asio::connect(socket, res.begin(), res.end());

	// setup HTTP GET request
	constexpr auto httpVersion = 11;
	http::request<http::string_body> req{http::verb::get, uri, httpVersion};
	req.set(http::field::host, host);
	req.set(http::field::user_agent, "myClient");

	// send HTTP request
	http::write(socket, req);

	// buffer for reading (must be persisted)
	boost::beast::flat_buffer buffer;
	
	// hold response
	http::response<http::dynamic_body> resp;

	// receive response
	http::read(socket, buffer, resp);

	std::cout << resp << "\n";

	// close socket
	boost::system::error_code error_code;
	socket.shutdown(tcp::socket::shutdown_both, error_code);

	if (error_code) {
		std::cerr << "Error occured when connection closed! error code: " << error_code;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
