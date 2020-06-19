
#include "gtest/gtest.h"
#include "reply.h"
#include <iostream>


class reply_test :public::testing::Test {
protected:
  std::unique_ptr<http::server::reply> reply_ = std::make_unique<http::server::reply>();
  std::vector<boost::asio::const_buffer> buffers;
};


TEST_F(reply_test, BasicTest) {
    reply_->status = http::server::reply::ok;    
    reply_->headers.resize(2);
    reply_->content = "test reply content";
    reply_->headers[0].name = "Content-Length";
    reply_->headers[0].value = std::to_string((reply_->content).length());
    reply_->headers[1].name = "Content-Type";
    reply_->headers[1].value = "text/plain";
    
    buffers = reply_->to_buffers();
    EXPECT_TRUE(buffers.size() == 11);
}


TEST_F(reply_test, ShorterTest) {
    reply_->status = http::server::reply::internal_server_error;    
    reply_->headers.resize(1);
    reply_->content = "test reply content";
    reply_->headers[0].name = "Content-Length";
    reply_->headers[0].value = std::to_string((reply_->content).length());
    
    buffers = reply_->to_buffers();
    EXPECT_TRUE(buffers.size() == 7);
}

TEST_F(reply_test, StockReply) {
    reply_ = http::server::reply::stock_reply(http::server::reply::bad_request);
    buffers = reply_->to_buffers();
    EXPECT_TRUE(buffers.size() == 11);
}

TEST_F(reply_test, RunThroughAllStocks) {
    for (int i = http::server::reply::ok; i != http::server::reply::service_unavailable; i++) {
      http::server::reply::status_type temp_status = static_cast<http::server::reply::status_type>(i);
      reply_ = http::server::reply::stock_reply(temp_status);
      buffers = reply_->to_buffers();
      EXPECT_TRUE(buffers.size() == 11);
    }
}