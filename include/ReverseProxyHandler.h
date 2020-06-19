#ifndef REVERSEPROXYHANDLER
#define REVERSEPROXYHANDLER
#include "RequestHandler.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <vector>
class ReverseProxyHandler : public RequestHandler
{
public:
  //std::unique_ptr<reply> handleRequest(const request &request);
  std::unique_ptr<reply> handleRequest(const request &request);
  static RequestHandler *Init(const NginxConfig &config)
  {
    // return a new reverse-proxy handler
    ReverseProxyHandler* handler = new ReverseProxyHandler;
    handler->mapReverseProxyPath = config.getMapReverseProxyPath();
    return handler;
  }
  std::string getHostName(std::string uri);
  std::vector<std::string> SeparatePath(std::string path);
  std::string modifyContent(std::string content, std::string prefix);
  std::string getContentType(std::string headers);
  http::server::reply::status_type getStatusCode(std::string headers);

private:
  std::unordered_map<std::string, std::string> mapReverseProxyPath;
  std::string prefix;
};

#endif