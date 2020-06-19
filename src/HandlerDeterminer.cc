#include "HandlerDeterminer.h"

HandlerDeterminer::HandlerDeterminer(NginxConfig inp_config) {
    config = inp_config;
    reqPathToFolderPathMap = config.get_paths_from_config();
    handlersAvaliable = config.getHandlerNames();
}

//The main function of the handlerDeterminer
//returns the appropriate handler for a given request
std::string HandlerDeterminer::pickHandler(request& req, RequestHandler*& requestHandler) {
    
    std::string hanTypeString = "";
    
    hanTypeString += req.uri[0];
    for (int i = 1; i < req.uri.size() && req.uri[i] != '/'; i++) {
        hanTypeString += req.uri[i];     
    }
    std::cout<< "determining handler" << std::endl; 
    /*if(req.method == "POST"){
            std::cout << " post method " << std::endl;
            requestHandler = handlers["KeyHandler"];
            return "KeyHandler";
            
    } */
    std::string handlerTypeName = getHandlerNameFromReqPath(hanTypeString);
    
    
   

    if (handlerTypeName == "") {
        //handler not found
        requestHandler = nullptr;
    }
    else
    {
        
        requestHandler = handlers[handlerTypeName];
    } 
    
    
    //REVERTED
    // requestHandler = handlers["ReverseProxyHandler"];
    // return "ReverseProxyHandler";
    return handlerTypeName;
    
}

//this is where the handlers get instantated using their static Init functions
void HandlerDeterminer::initializeHandlers() {
    //loop though all of the handler names specified in the config file and initialize them appropriately
    for (std::unordered_map<std::string, std::string>::iterator it = handlersAvaliable.begin(); it != handlersAvaliable.end(); it++) {
        if (it->second == "EchoHandler") {
            handlers["EchoHandler"] = EchoHandler::Init(config);
        }
        else if (it->second == "StaticHandler") {
            handlers["StaticHandler"] = StaticHandler::Init(config);
            std::cout << "Static Handler " << std::endl; 
        }

        else if (it->second == "ErrorHandler") {
            handlers["ErrorHandler"] = RequestHandlerError::Init(config);
        }
        else if (it->second == "ReverseProxyHandler") {
            handlers["ReverseProxyHandler"] = ReverseProxyHandler::Init(config);
        }
        else if (it->second == "HealthHandler") {
            handlers["HealthHandler"] = HealthHandler::Init(config);
        }
        else if (it->second == "KeyHandler") {
            handlers["KeyHandler"] = KeyHandler::Init(config);
        }
    }
}

//given a name, return the corresponding handler if it has been initialized
// if it has not been initialzed then jsut return nullptr
RequestHandler* HandlerDeterminer::getHandlerInstance(std::string handlerName) {
    if (handlers.find(handlerName) != handlers.end()) {
        return handlers[handlerName];
    }
    else
    {
        return nullptr;
    }
}

//return handler name like "EchoHandler" when provided the uri path
//that matches it in the config file
std::string HandlerDeterminer::getHandlerNameFromReqPath(std::string reqPath) {
    std::unordered_map<std::string, std::string>::iterator it = handlersAvaliable.find(reqPath);
    if (it == handlersAvaliable.end()) {
        return "";
    }
    else
    {
        return it->second;
    }
}


//delete the handlers that had been declared on the heap.
//I think that you still have to do this even though they are static
HandlerDeterminer::~HandlerDeterminer() {
    for (std::unordered_map<std::string, RequestHandler*>::iterator it = handlers.begin(); it != handlers.end(); it++) {
        delete it->second;
    }
}
