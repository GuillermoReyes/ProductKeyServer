#ifndef SERVER
#define SERVER

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
using boost::asio::ip::tcp;
#include "config_parser.h"
#include "session.h"

class server
{
public:
  server(boost::asio::io_service& io_service, short port, NginxConfig& config, HandlerDeterminer& handlerDeterminer);

//private:
  void start_accept();

  void handle_accept(session* new_session,
      const boost::system::error_code& error);

  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
  NginxConfig config_;
  HandlerDeterminer* handlerDeterminer_;
};

#endif