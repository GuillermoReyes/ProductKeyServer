// #include "gtest/gtest.h"
// #include "session.h"
// #include "gmock/gmock.h"
// #include "config_parser.h"
// #include "RequestHandler.h"
// #include "EchoHandler.h"
// #include "reply.h"
// #include "request.h"

// class SessionTest : public ::testing::Test {
// protected:
//   boost::asio::io_service io_service2;
//   session* new_session = new session(io_service2);
// };

// TEST_F(SessionTest, BasicRequest) {
//   strcpy(new_session->data_, "input string to session.");
//   //new_session->handle_read(boost::system::error_code(), strlen(new_session->data_));
//   EXPECT_TRUE(new_session->reply_.content == "400");
// }



/**
 * Unit tests for session request callbacks.
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "session.h"
#include "config_parser.h"
#include "RequestHandler.h"


TEST(SessionTest, CheckProperReply) {
  std::cout << "TESTOMGMSGODSMAGOSDGMOSDGMSDO" << std::endl;
  NginxConfig config;
  HandlerDeterminer handlerDeterminer(config);
  handlerDeterminer.handlers["EchoHandler"] = EchoHandler::Init(config);
  handlerDeterminer.handlersAvaliable["/echo"] = "EchoHandler";
  boost::asio::io_service io_service;
  session sesh(io_service, config, handlerDeterminer);
  std::string inp = "GET /echo HTTP/1.1\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nConnection: close\r\n\r\n";
  strcpy(sesh.data_, inp.c_str());
  sesh.handle_read(boost::system::error_code(), strlen(sesh.data_));
  EXPECT_EQ(sesh.reply_->status, reply::ok);
  delete handlerDeterminer.handlers["EchoHandler"];
}














