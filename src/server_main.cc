//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
using boost::asio::ip::tcp;
#include "config_parser.h"
#include "server.h"
#include "session.h"
#include "logger.h"
#include <thread>
#include <boost/thread.hpp>

void my_log_handler(int s){
    Logger * logger = Logger::getLogger();
    logger->log_shutdown();
    exit(1);
}

int main(int argc, char* argv[])
{
  try
  {
    /* LOGGING */
    Logger *logger = Logger::getLogger();
    struct sigaction sigIntLogHandler;
    sigIntLogHandler.sa_handler = my_log_handler;
    sigemptyset(&sigIntLogHandler.sa_mask);
    sigIntLogHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntLogHandler, NULL);

    if (argc != 2)
    {
      std::cerr << "Usage: async_tcp_echo_server " << argv[0] << " <config_file>\n"; //fixed error report to show config file instead of port
      logger->log_error_write("ERROR: Wrong usage port.\n"); //log appropriately
      return 1;
    }

    NginxConfigParser m_parser;
    NginxConfig m_config;

    /* LOGGING */
    logger->log_trace_write("Parsing config file ...\n");
    logger->log_trace_write("PARSING DONE  ...\n");

    if (!m_parser.Parse(argv[1], &m_config))
    {
      std::cerr << "ERROR: Parsing has failed ...\n";
      logger->log_error_write("ERROR: Parsing has failed.\n"); //log error
      return 1;
    }

    int portno = m_config.get_port_from_config();
    if(portno == -1) //get_port_from_config() is STILL NOT DEFINED
    {
      std::cout << "portnum is: " << portno << std::endl; 
      std::cerr << "ERROR: Port number is not valid\n";
      logger->log_error_write("ERROR: Invalid port number in config file.\n"); //log error
      return 1;
    }

    boost::asio::io_service io_service;

    /* LOGGING */
    logger->log_server_init();
    logger->log_trace_write("Starting server on port: " + std::to_string(portno));


    using namespace std; // For atoi.
    HandlerDeterminer handlerDeterminer(m_config);
    server s(io_service, static_cast<short>(portno), m_config, handlerDeterminer); //changed port to portno in this line

    //multithreading happens here
    //NEW
    int thread_pool_size_ = 5;
    std::vector<boost::shared_ptr<boost::thread>> threads;
    for (std::size_t i = 0; i < thread_pool_size_; ++i)
    {
      boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service)));
      threads.push_back(thread);
    }

    for (std::size_t i = 0; i < threads.size(); ++i)
    {
      threads[i]->join();
    }

    //OLD
    // io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
