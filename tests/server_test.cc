#include "gtest/gtest.h"
#include "server.h"
#include "session.h"
#include "gmock/gmock.h"

using ::testing::AtLeast;
using namespace ::testing; 

class MockSession : public session{
  public:
    MockSession(boost::asio::io_service& io_service__, NginxConfig& config, HandlerDeterminer& handlerDeterminer) : session(io_service__, config, handlerDeterminer) {}
    //MOCK_METHOD0(handle_read, void());
    void start() {
          changed = true;
          return;
      }
    // MOCK_METHOD0(handle_write, void());
    // MOCK_METHOD0(start, void());
    bool changed = false;
};


// class ServerTest : public ::testing::Test {
// protected:
//     MockSession session;
// };

// TEST_F(ServerTest, BasicTest) {
//     int port = 8001;
//     EXPECT_CALL(session, handle_read()).Times(AtLeast(1));
//     server serv(session.io_service, port);
//     EXPECT_TRUE(1);
// }



// TEST(ServerTest, BasicServerTest) {
//     MockSession sesh;
//     strcpy(sesh.data_, "test");
//     int port = 8001;
//     EXPECT_CALL(sesh, handle_read()).Times(AtLeast(1));
//     server serv(sesh.io_service, port);
//     EXPECT_TRUE(1);
// }

TEST(ServerTest, BasicServerTest) {
    boost::asio::io_service io_service;
    NginxConfig config;
    HandlerDeterminer handlerDeterminer(config);
    MockSession sesh(io_service, config, handlerDeterminer);
    strcpy(sesh.data_, "test");
    int port = 8004;
    //EXPECT_CALL(sesh, handle_read()).Times(AtLeast(1));
    server serv(io_service, port, config, handlerDeterminer);
    serv.handle_accept(&sesh, boost::system::error_code()); 
    //EXPECT_TRUE(sesh.reply_.status == reply::ok);
    EXPECT_TRUE(sesh.changed);
}