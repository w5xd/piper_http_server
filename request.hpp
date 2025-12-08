//
// request.hpp
// Cribbed from HTTP Server 3 at: https://www.boost.org/doc/libs/latest/doc/html/boost_asio/examples/cpp11_examples.html
//
#ifndef HTTP_SERVER3_REQUEST_HPP
#define HTTP_SERVER3_REQUEST_HPP

#include <string>
#include <vector>
#include "header.hpp"

namespace http {
namespace server3 {

/// A request received from a client.
struct request
{
  std::string method;
  std::string uri;
  int http_version_major;
  int http_version_minor;
  std::vector<header> headers;
  void reset()
  {
    method.clear();
    uri.clear();
    headers.clear();
  }
};

} // namespace server3
} // namespace http

#endif // HTTP_SERVER3_REQUEST_HPP
