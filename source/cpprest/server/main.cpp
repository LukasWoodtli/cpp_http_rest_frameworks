#include <functional>
#include <condition_variable>
#include <mutex>
#include <iostream>

#include <cpprest/uri.h>
#include <cpprest/http_listener.h>

static void handleGet( web::http::http_request message) ;

static void handleUserInterrupt(int signal);
static void waitForUserInterrupt();

static std::condition_variable condition;
static std::mutex mutex;

int main()
{
	signal(SIGINT, handleUserInterrupt);

	web::uri endpointURI ("http://localhost:8900/api/");
	web::uri_builder endpointBuilder;
	endpointBuilder.set_scheme(endpointURI.scheme());

	endpointBuilder.set_port(endpointURI.port());
	endpointBuilder.set_path(endpointURI.path());
	endpointBuilder.set_host(endpointURI.host());

	using web::http::experimental::listener::http_listener;
	auto uri = endpointBuilder.to_uri();
	auto listener = http_listener(uri);

	listener.support(web::http::methods::GET, &handleGet);
	listener.open().wait();

	std::cout << "Waiting for HTTP requests. Quit with ctrl-c\n";
	waitForUserInterrupt();
	listener.close().wait();
}

void handleGet(web::http::http_request message) {
	auto response = "Hello!";
	message.reply(web::http::status_codes::OK, response);
}


void handleUserInterrupt(int signal){
    if (signal == SIGINT) {
        condition.notify_one();
    }
}

void waitForUserInterrupt() {
    std::unique_lock<std::mutex> lock{mutex};
    condition.wait(lock);
    lock.unlock();
}
