#include <cstdlib>
#include <vector>
#include <thread>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/bind_executor.hpp>
#include <boost/asio/strand.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;


using request = http::request<http::string_body>;
using response = http::response<http::string_body>;

void fail(boost::system::error_code error_code)
{
    throw std::logic_error ( error_code.message() );
}

// return a response for a bad request
response bad_request(const request& req, const std::string& why ) {
        response res {http::status::bad_request, req.version() };
        res.set ( http::field::server, "myServer" );
        res.set ( http::field::content_type, "text/html" );
        res.keep_alive ( req.keep_alive() );
        res.body() = why;
        res.prepare_payload();
        return res;
}

void handle_request ( const request req,
                      std::function<void(response)> send )
{
    // Make sure we can handle the method
    if ( req.method() != http::verb::get ) {
			  auto res = bad_request(req, "Unknown HTTP-method!\n");
        send(res);
				return;
    }

    // respond to GET
    response res {http::status::ok, req.version() };
    res.set ( http::field::server, "myServer" );
    res.set ( http::field::content_type, "text/html" );
    std::string msg("Hello from beast!\nTarget: ");
    msg += req.target().to_string();
		msg += "\n";
		res.body() = msg;
    res.keep_alive ( req.keep_alive() );
    res.prepare_payload();
    send(res);
}

class session : public std::enable_shared_from_this<session>
{
    tcp::socket m_socket;
    boost::asio::strand<boost::asio::io_context::executor_type> m_strand;
    boost::beast::flat_buffer m_buffer;
    request m_req;
    std::shared_ptr<response> m_res;
    std::function<void(response)> m_lambda;

public:
    // take ownership fo socket
    explicit session ( tcp::socket socket )
        : m_socket ( std::move ( socket ) ),
          m_strand ( m_socket.get_executor() )
    {
			m_lambda = [this]( response msg )
        {
            auto sp = std::make_shared<response>(msg);

            this->m_res = sp;

            // write response
            http::async_write ( this->m_socket,
                                *sp,
                                boost::asio::bind_executor ( this->m_strand,
                                        std::bind ( &session::on_write,
                                                    this->shared_from_this(),
                                                    std::placeholders::_1,
                                                    std::placeholders::_2,
                                                    sp->need_eof() ) ) );
        };
    }

    // start async operation
    void run()
    {
        do_read();
    }

    void do_read()
    {
        // read request
        http::async_read ( m_socket, m_buffer, m_req,
                           boost::asio::bind_executor ( m_strand,
                                   std::bind (
                                       &session::on_read,
                                       shared_from_this(),
                                       std::placeholders::_1,
                                       std::placeholders::_2 ) ) );
    }

    void on_read ( boost::system::error_code ec,
                   std::size_t bytesTransfered )
    {
        ( void ) bytesTransfered;

        // closed connection
        if ( ec == http::error::end_of_stream ) {
            return do_close();
        }

        if ( ec ) {
            fail(ec);
        }

        // send response
        handle_request (m_req, m_lambda);
    }

    void on_write ( boost::system::error_code ec,
                    std::size_t bytesTransfered,
                    bool close )
    {
        ( void ) bytesTransfered;

        if ( ec ) {
            fail(ec);
        }

        if ( close ) {
            return do_close();
        }

        // delete response, not used anymore
        m_res = nullptr;

        // read another request
        do_read();
    }

    void do_close()
    {
        boost::system::error_code ec;
        m_socket.shutdown ( tcp::socket::shutdown_send, ec );

        if ( ec ) {
            fail(ec);
        }

    }
};


class listener : public std::enable_shared_from_this<listener>
{
    tcp::acceptor m_acceptor;
    tcp::socket m_socket;

public:
    listener ( boost::asio::io_context& ioc,
               tcp::endpoint endpoint )
        : m_acceptor ( ioc ), m_socket ( ioc )
    {

        boost::system::error_code error_code;

        // open acceptor
        m_acceptor.open ( endpoint.protocol(), error_code );
        if ( error_code ) {
            fail(error_code);
        }

        // bind to server address
        m_acceptor.bind ( endpoint, error_code );
        if ( error_code ) {
            fail(error_code);
        }

        // listening for connections
        m_acceptor.listen ( boost::asio::socket_base::max_listen_connections, error_code );
        if ( error_code ) {
            fail(error_code);
        }
    }


    void run()
    {
        if ( !m_acceptor.is_open() ) {
					throw std::logic_error("Acceptor not open!");
        }
        do_accept();
    }


    void do_accept()
    {
        m_acceptor.async_accept ( m_socket,
                                  std::bind ( &listener::on_accept,
                                              shared_from_this(),
                                              std::placeholders::_1 ) );
    }


    void on_accept ( boost::system::error_code ec )
    {
        if ( ec ) {
            fail(ec);
        } else {
            // create session and run it
            std::make_shared<session> ( std::move ( m_socket ) )->run();
        }

        // accept anoter connection
        do_accept();
    }
};

int main()
{
    auto const address = boost::asio::ip::make_address ( "0.0.0.0" );
    const unsigned short port = 7890;
    const int numThreads = 10;

    // context required for I/O
    boost::asio::io_context io_context {numThreads};

    // create and lauch a listenign port
    std::make_shared<listener>(io_context, tcp::endpoint {address, port})->run();

    // run I/O service with given number of threads
    std::vector<std::thread> v;
    v.reserve ( numThreads - 1 );
    for ( auto i = numThreads - 1; i > 0; --i ) {
        v.emplace_back ( [&io_context] {io_context.run();} );
    }
    io_context.run();

    return EXIT_SUCCESS;
}
