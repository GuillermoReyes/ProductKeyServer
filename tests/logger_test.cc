#include "gtest/gtest.h"
#include "logger.h"
#include <iostream>
#include <fstream>
#include "request.h"
#include "request_parser.h"
#include <boost/asio.hpp>
// #include <session.h>

class LoggerTest : public :: testing::Test{
protected:
  Logger * logger = Logger::getLogger();
  char c;
  std::ifstream logfile;
  std::string Message = "message";
};

TEST_F(LoggerTest, log_server_init){
	std::string correct_msg = "Trace: Server has been initialized\n";
	logger->log_server_init();
	logfile.open("../log/SYSLOG_0.log");
	std::string log_body;
  while (logfile.get(c)) log_body += c;
  logfile.close();
  int start = log_body.rfind("]") + 2;
  std::string test_msg = log_body.substr(start);
  EXPECT_TRUE(test_msg == correct_msg);
}

TEST_F(LoggerTest, log_trace_write){
	std::string correct_msg = "Trace: message\n";
  logger->log_trace_write(Message);
	logfile.open("../log/SYSLOG_0.log");
	std::string log_body;
  while (logfile.get(c)) log_body += c;
  logfile.close();
  int start = log_body.rfind("]") + 2;
  std::string test_msg = log_body.substr(start);
  EXPECT_TRUE(test_msg == correct_msg);
}

TEST_F(LoggerTest, log_error_write){
	std::string correct_msg = "Error: message\n";
	logger->log_error_write(Message);
  logfile.open("../log/SYSLOG_0.log");
  std::string log_body;
  while (logfile.get(c)) log_body += c;
  logfile.close();
  int start = log_body.rfind("]") + 2;
  std::string test_msg = log_body.substr(start);
  EXPECT_TRUE(test_msg == correct_msg);
}

TEST_F(LoggerTest, log_debug_write){
	std::string correct_msg = "Debug: message\n";
	logger->log_debug_write(Message);
	logfile.open("../log/SYSLOG_0.log");
	std::string log_body;
  while (logfile.get(c)) log_body += c;
  logfile.close();
  int start = log_body.rfind("]") + 2;
  std::string test_msg = log_body.substr(start);
  EXPECT_TRUE(test_msg == correct_msg);
}

TEST_F(LoggerTest, log_warning_write){
	std::string correct_msg = "Warning: message\n";
	logger->log_warning_write(Message);
	logfile.open("../log/SYSLOG_0.log");
	std::string log_body;
  while (logfile.get(c)) log_body += c;
  logfile.close();
  int start = log_body.rfind("]") + 2;
  std::string test_msg = log_body.substr(start);
  EXPECT_TRUE(test_msg == correct_msg);
}

TEST_F(LoggerTest, log_shutdown){
  std::string correct_msg = "Warning: Shutting down the server...\n";
  logger->log_shutdown();
  logfile.open("../log/SYSLOG_0.log");
  std::string log_body;
  while (logfile.get(c)) log_body += c;
  logfile.close();
  int start = log_body.rfind("]") + 2;
  std::string test_msg = log_body.substr(start);
  EXPECT_TRUE(test_msg == correct_msg);
}

/* CITATIONS: test adapted from UPE tutoring session */
