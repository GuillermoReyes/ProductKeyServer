#ifndef HEALTHHANDLER
#define HEALTHHANDLER
#include "RequestHandler.h"

class HealthHandler : public RequestHandler {
public:   
    std::unique_ptr<reply> handleRequest(const request &request_);

    static RequestHandler* Init(const NginxConfig& config) {
        //return a new echohandler
        return new HealthHandler();
    }
};



#endif