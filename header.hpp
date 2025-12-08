//
// header.hpp
// Cribbed from HTTP Server 3 at: https://www.boost.org/doc/libs/latest/doc/html/boost_asio/examples/cpp11_examples.html
//

#ifndef HTTP_SERVER3_HEADER_HPP
#define HTTP_SERVER3_HEADER_HPP

#include <string>

namespace http {
namespace server3 {

struct header
{
  std::string name;
  std::string value;
};

} // namespace server3
} // namespace http

#endif // HTTP_SERVER3_HEADER_HPP
