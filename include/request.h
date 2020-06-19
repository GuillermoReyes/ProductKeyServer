//
// request.hpp
// ~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>
#include <vector>
#include "header.h"
#include <cassert>
#include <map>

namespace http {
namespace server {

/// A request received from a client.
class request
{
public:
  std::string method;
  std::string uri;
  int http_version_major;
  int http_version_minor;
  std::vector<header> headers;
  std::map<std::string, std::string> headerMap;

  std::string toString() const;
  void fillMapFromHeaders();
  std::string body;
};

} // namespace server
} // namespace http

#endif // HTTP_REQUEST_HPP