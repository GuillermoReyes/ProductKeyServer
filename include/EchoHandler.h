#ifndef ECHOHANDLER
#define ECHOHANDLER
#include "RequestHandler.h"

class EchoHandler : public RequestHandler {
public:   
    std::unique_ptr<reply> handleRequest(const request &request_);

    static RequestHandler* Init(const NginxConfig& config) {
        //return a new echohandler
        return new EchoHandler();
    }
};



#endif