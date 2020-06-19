#ifndef SESSION
#define SESSION

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
using boost::asio::ip::tcp;
#include "config_parser.h"
#include "RequestHandler.h"

#include "request_parser.h"
#include "request.h"
#include "reply.h"
#include "header.h"
#include "EchoHandler.h"
#include "HandlerDeterminer.h"

using boost::asio::ip::tcp;
using http::server::request;
using http::server::request_parser;
using http::server::reply; 


class session : public std::enable_shared_from_this<session>
{
public:
  session(boost::asio::io_service& io_service, NginxConfig config, HandlerDeterminer& HandlerDeterminer);

  tcp::socket& socket();

  tcp::socket& socket_remote();

  virtual void start();

  void finishIncompleteRead(int charsReadAlready);

//private: //make everything public for testing
  //where most of the request processing happens
  void handle_read(const boost::system::error_code& error,
      size_t bytes_transferred);

  void handle_write(const boost::system::error_code& error);

  

  tcp::socket socket_;

  tcp::socket socket_remote_;

  enum { max_length = 1024 };
  char data_[max_length];

  //added
  static std::map<std::string, std::vector<int>> request_received_;

  request request_;
  request_parser request_parser_;
  std::unique_ptr<reply> reply_;
  NginxConfig config_;
  int num_requests; 

  HandlerDeterminer* handlerDeterminer_;
};

#endif