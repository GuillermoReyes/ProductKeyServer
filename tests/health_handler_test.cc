#include "gtest/gtest.h"
#include "HealthHandler.h"

class HealthHandlerTest : public :: testing::Test{
protected: 
  void SetUp() override {
    NginxConfig emptyConfig;
    healthHandler = HealthHandler::Init(emptyConfig);
  }
  RequestHandler* healthHandler;
    std::unique_ptr<reply> rep;
    request req;
};


//testing the determineHandler function
TEST_F(HealthHandlerTest, basicHealthHandlerTest) {
    rep = healthHandler->handleRequest(req);
    EXPECT_EQ(rep->status, reply::ok);
};