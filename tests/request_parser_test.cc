
#include "gtest/gtest.h"
#include "request_parser.h"
#include "request.h"


class RequestParserTest : public :: testing::Test{
protected:
  http::server::request_parser request_parser;
  http::server::request req;
  http::server::request_parser::result_type result;
};

//Typical valid request
TEST_F(RequestParserTest, GetRequest) {
  char req_str[1024] = "GET /echo HTTP/1.1\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, req_str, req_str + strlen(req_str));
  EXPECT_TRUE(result == http::server::request_parser::good);
}

//Typical valid request
TEST_F(RequestParserTest, PostRequest) {
  char req_str[1024] = "POST / HTTP/1.1\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, req_str, req_str + strlen(req_str));
  EXPECT_TRUE(result == http::server::request_parser::good);
}

//Invalid request ending format
TEST_F(RequestParserTest, InvalidEnding) {
  char req_str[1024] = "GET / HTTP/1.1\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nConnection: close\r\n";;
  std::tie(result, std::ignore)  = request_parser.parse(req, req_str, req_str + strlen(req_str));
  EXPECT_TRUE(result != http::server::request_parser::good);
}

//Empty request is invalid
TEST_F(RequestParserTest, Empty) {
  char req_str[1024] = "";
  std::tie(result, std::ignore)  = request_parser.parse(req, req_str, req_str + strlen(req_str));
  EXPECT_TRUE(result != http::server::request_parser::good);
}

//Invalid body but valid ending is invalud
TEST_F(RequestParserTest, NoBody) {
  char req_str[1024] = "NOBODY\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, req_str, req_str + strlen(req_str));
  EXPECT_TRUE(result != http::server::request_parser::good);
}


TEST_F(RequestParserTest, RandBadRequest1) {
  char req_str[1024] = "POST / HTTP/1.1          \r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, req_str, req_str + strlen(req_str));
  EXPECT_TRUE(result != http::server::request_parser::good);
}

TEST_F(RequestParserTest, RandBadRequest2) {
  char req_str[1024] = "\r\r/ HTTP/1.1\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, req_str, req_str + strlen(req_str));
  EXPECT_TRUE(result != http::server::request_parser::good);
}

TEST_F(RequestParserTest, RandBadReques3) {
  char req_str[1024] = "\n / HTTP/1.1\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, req_str, req_str + strlen(req_str));
  EXPECT_TRUE(result != http::server::request_parser::good);
}

TEST_F(RequestParserTest, RandBadRequest4) {
  char req_str[1024] = "POST / HTTP/1.1\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nConnection: close\r\r\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, req_str, req_str + strlen(req_str));
  EXPECT_TRUE(result != http::server::request_parser::good);
}

TEST_F(RequestParserTest, RandBadRequest5) {
  char req_str[1024] = "POST / HTTP/11\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, req_str, req_str + strlen(req_str));
  EXPECT_TRUE(result != http::server::request_parser::good);
}

TEST_F(RequestParserTest, RandBadRequest6) {
  char req_str[1024] = "POST / HTTP\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, req_str, req_str + strlen(req_str));
  EXPECT_TRUE(result != http::server::request_parser::good);
}

TEST_F(RequestParserTest, RandBadRequest7) {
  char req_str[1024] = "POST / HTTP/1.1\r\n\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, req_str, req_str + strlen(req_str));
  EXPECT_TRUE(result == http::server::request_parser::good);
}

TEST_F(RequestParserTest, RandBadRequest8) {
  char req_str[1024] = "HTTP/1.1\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, req_str, req_str + strlen(req_str));
  EXPECT_TRUE(result != http::server::request_parser::good);
}


//testing request format
TEST_F(RequestParserTest, requestFormat1) {
  char req_str[1024] = "GET /echo HTTP/1.1\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, req_str, req_str + strlen(req_str));
  EXPECT_TRUE(result == http::server::request_parser::good);
  EXPECT_EQ(req.method, "GET");
  EXPECT_EQ(req.uri, "/echo");
  EXPECT_EQ(req.headerMap["Host"], "www.tutorialspoint.com");
}

//testing request format
TEST_F(RequestParserTest, requestFormat2Body) {
  char req_str[1024] = "GET /echo HTTP/1.1\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nConnection: close\r\n\r\n";
  std::tie(result, std::ignore)  = request_parser.parse(req, req_str, req_str + strlen(req_str));
  EXPECT_TRUE(result == http::server::request_parser::good);
  EXPECT_EQ(req.body, "GET /echo HTTP/1.1\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nConnection: close\r\n\r\n");
}

// GET /hello.htm HTTP/1.1
// User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)
// Host: www.tutorialspoint.com
// Accept-Language: en-us
// Accept-Encoding: gzip, deflate
// Connection: Keep-Alive