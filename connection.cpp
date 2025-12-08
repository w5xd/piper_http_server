//
// connection.cpp
// Cribbed from HTTP Server 3 at: https://www.boost.org/doc/libs/latest/doc/html/boost_asio/examples/cpp11_examples.html
//

#include "connection.hpp"
#include <utility>
#include "request_handler.hpp"

namespace http {
namespace server3 {

    connection::connection(boost::asio::ip::tcp::socket socket,
        request_handler& handler)
        : socket_(std::move(socket))
        , request_handler_(handler)
        , keep_alive(false)
    {}

    connection::~connection()
    {}

    void connection::start()
    {
        do_read();
    }

    void connection::do_read()
    {
        keep_alive = false;
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(buffer_),
            [this, self](boost::system::error_code ec, std::size_t bytes_transferred)
            {
                if (!ec)
                {
                    request_parser::result_type result;
                    std::tie(result, std::ignore) = request_parser_.parse(
                        request_, buffer_.data(), buffer_.data() + bytes_transferred);

                    if (result == request_parser::good)
                    {
                        keep_alive = request_handler_.handle_request(request_, reply_, request_parser_.content());
                        do_write();
                    }
                    else if (result == request_parser::bad)
                    {
                        reply_ = reply::stock_reply(reply::bad_request);
                        do_write();
                    }
                    else
                    {
                        do_read();
                    }
                }

                // If an error occurs then no new asynchronous operations are
                // started. This means that all shared_ptr references to the
                // connection object will disappear and the object will be
                // destroyed automatically after this handler returns. The
                // connection class's destructor closes the socket.
            });
    }

    void connection::do_write()
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, reply_.to_buffers(),
            [this, self](boost::system::error_code ec, std::size_t)
            {
                if (!ec)
                {   // succeeded
                    if (!keep_alive)
                    {
                        // Initiate graceful connection closure.
                        boost::system::error_code ignored_ec;
                        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                            ignored_ec);
                    }
                    else
                        do_read();
                }

                // else...
                // No new asynchronous operations are started. This means that
                // all shared_ptr references to the connection object will
                // disappear and the object will be destroyed automatically after
                // this handler returns. The connection class's destructor closes
                // the socket.
            });
        request_parser_.reset();
        request_.reset();
    }

} // namespace server3
} // namespace http
