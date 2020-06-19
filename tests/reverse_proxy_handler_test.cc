#include "gtest/gtest.h"
#include "ReverseProxyHandler.h"
#include <vector>
#include "reply.h"

using namespace std;

class ReverseProxyHandlerTest : public :: testing::Test{
protected:
  ReverseProxyHandler handler;
  RequestHandler* handler2;
  http::server::request request;
  NginxConfig config;
  NginxConfigParser parser;
  void SetUp() override {
    parser.Parse("staticConfigLocal", &config);
    handler2 = ReverseProxyHandler::Init(config);
  }
};

TEST_F(ReverseProxyHandlerTest, getHostName) {
  string uri = "https://www.ucla.edu/admission";
	string hostName = handler.getHostName(uri);
  EXPECT_EQ(hostName, "www.ucla.edu");
}

TEST_F(ReverseProxyHandlerTest, SeparatePath) {
  string path = "/static/foo/index.html";
  vector<string> paths = handler.SeparatePath(path);
  EXPECT_EQ(paths.size(), 2);
  EXPECT_EQ(paths[0], "/static");
  EXPECT_EQ(paths[1], "/foo/index.html");
}

TEST_F(ReverseProxyHandlerTest, modifyContent) {
  string path = "img src=\"/img/welcometo.gif\"";
	string prefix = "/cpp";
	string result = handler.modifyContent(path, prefix);
  EXPECT_EQ(result, "img src=\"/cpp/img/welcometo.gif\"");
}

TEST_F(ReverseProxyHandlerTest, getContentType) {
  string response = "HTTP/1.1 301 Moved Permanently\r\nContent-Type: text/html\r\n";
	EXPECT_EQ(handler.getContentType(response), "text/html");
}

TEST_F(ReverseProxyHandlerTest, getStatusCode) {
  string response = "HTTP/1.1 301 Moved Permanently\r\nContent-Type: text/html\r\n";
	EXPECT_EQ(handler.getStatusCode(response), http::server::reply::moved_permanently);
}

TEST_F(ReverseProxyHandlerTest, handleRequest) {
  request.uri = "/ucla/js/modernizr.js";
  unique_ptr<reply> reply_ = handler2->handleRequest(request);
  EXPECT_EQ(reply_->headers[0].name, "Content-Length");
  EXPECT_EQ(reply_->headers[0].value, to_string(reply_->content.length()));
}