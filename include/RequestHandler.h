
#ifndef REQUEST_HANDLER
#define REQUEST_HANDLER


#include "reply.h"
#include "request.h"
#include "config_parser.h"


using http::server::request;
using http::server::reply;

//virtual request handler that can be overwritten to serve any request functionality
//currently it is defaultly overwritten to an echo handler
//willl have to write some sort of function that determines what kind of handler is needed
class RequestHandler {
public:
    //takes a request and outputs a filled out reply
    virtual std::unique_ptr<reply> handleRequest(const request &request_) = 0;
    //REVERTED //virtual void handleRequest(const request &request_, reply &reply_) {};

    //returns a pointer to a new handler
    static RequestHandler* Init(const NginxConfig& config);
};


#endif