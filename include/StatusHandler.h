#ifndef STATUSHANDLER
#define STATUSHANDLER
#include "RequestHandler.h"
#include "HandlerDeterminer.h"
#include "config_parser.h"

class StatusHandler : public  RequestHandler {
    
public:
    StatusHandler();
    static RequestHandler* Init(const NginxConfig& config){
        return new StatusHandler();
    }
    
    std::unique_ptr<reply> handleRequest(const request &request_) override;
    
    
    
    
};

#endif
