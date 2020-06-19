#ifndef REQUEST_HANDLER_ERROR_H
#define REQUEST_HANDLER_ERROR_H

#include <iostream>

#include "RequestHandler.h"

class RequestHandlerError : public RequestHandler {
public:
    explicit RequestHandlerError(const NginxConfig &config) {}    //for sibling class
    std::unique_ptr<reply> handleRequest(const request &request_) noexcept override;
    std::string requestToString(const request &request_);
    
    static RequestHandler* Init(const NginxConfig& config) {
      return NULL; 
    }
};

#endif  // REQUEST_HANDLER_ERROR_H
