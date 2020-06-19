#include "gtest/gtest.h"
#include "HandlerDeterminer.h"

class HandlerDeterminerTest : public :: testing::Test{
protected: 
  void SetUp() override {
    parser.Parse("staticConfigLocal", &config);
    handlerDeterminer  = new HandlerDeterminer(config);
    handlerDeterminer->initializeHandlers();
  }
  NginxConfig config;
  NginxConfigParser parser;
  HandlerDeterminer* handlerDeterminer;
  http::server::request req;
  RequestHandler* requestHandler;
};


//testing the determineHandler function
TEST_F(HandlerDeterminerTest, DetermineHandlerEcho) {
  req.method = "GET";
  req.uri = "/echo";
  RequestHandler* requestHandler = nullptr;
  std::string handlerName = handlerDeterminer->pickHandler(req, requestHandler);
  EXPECT_EQ(handlerName, "EchoHandler");
  EXPECT_NE(requestHandler, nullptr);
}

//testing the determineHandler function
TEST_F(HandlerDeterminerTest, DetermineHandlerStatic) {
  req.method = "GET";
  req.uri = "/static1";
  RequestHandler* requestHandler = nullptr;
  std::string handlerName = handlerDeterminer->pickHandler(req, requestHandler);
  EXPECT_EQ(handlerName, "StaticHandler");
  EXPECT_NE(requestHandler, nullptr);
}

//testing the determineHandler function
TEST_F(HandlerDeterminerTest, FailToFindHandler) {
  req.method = "GET";
  req.uri = "/nonexist";
  RequestHandler* requestHandler = nullptr;
  std::string handlerName = handlerDeterminer->pickHandler(req, requestHandler);
  EXPECT_EQ(handlerName, "");
  EXPECT_EQ(requestHandler, nullptr);
}


TEST_F(HandlerDeterminerTest, GetHandlerInstanceTest) {
  EXPECT_NE(handlerDeterminer->getHandlerInstance("EchoHandler"), nullptr);
  EXPECT_EQ(handlerDeterminer->getHandlerInstance("nonexist"), nullptr);
}
