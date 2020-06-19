#ifndef HANDLERDETERMINER
#define HANDLERDETERMINER
#include "RequestHandler.h"
#include "EchoHandler.h"
#include "StaticHandler.h"
#include "ErrorHandler.h"
#include "ReverseProxyHandler.h"
#include "config_parser.h"
#include "ReverseProxyHandler.h"
#include "HealthHandler.h"
#include "KeyHandler.h"

class HandlerDeterminer {
public:
    HandlerDeterminer(NginxConfig inp_config);
    ~HandlerDeterminer();
    void initializeHandlers();
//private:
    //returns the correct instantated handler based on the request 
    std::string pickHandler(request& req, RequestHandler*& requestHandler);

    //map from uri root path to folder root path for the static handler
    std::unordered_map<std::string, std::string> reqPathToFolderPathMap;
    
    //the input config
    NginxConfig config;
    
    //map from req path to handler name (like "EchoHandler")
    std::unordered_map<std::string, std::string> handlersAvaliable;
    
    //map from handlerName to handler instance
    std::unordered_map<std::string, RequestHandler*> handlers;
    
    //returns the instantated handler instance from handler name if it exists
    RequestHandler* getHandlerInstance(std::string name);
    
    //returns the handler name from the uri path if it exists
    std::string getHandlerNameFromReqPath(std::string reqPath);
};




#endif
