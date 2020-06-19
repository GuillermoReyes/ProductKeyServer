//Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//https://www.boost.org/doc/libs/1_65_1/doc/html/boost_asio/example/cpp11/http/server/request_handler.hpp
#ifndef STATICHANDLER
#define STATICHANDLER
#include "RequestHandler.h"
#include <unordered_map>
#include "mime_types.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>


class StaticHandler : public RequestHandler {

public:   
    //we might actually have to pass in the config file as well but not sure yet
    StaticHandler(std::string root_path):base_(root_path){
        
    }
    static RequestHandler* Init(const NginxConfig& config) {
      std::string prefix = "/";
      for (int i = 0; i < config.statements_.size(); ++i) {
        if (config.statements_[i]->tokens_[0] == "root" &&
                config.statements_[i]->tokens_.size() == 2) {
            prefix = config.statements_[i]->tokens_[1];
        }
    }
      return new StaticHandler(prefix);

    }

    //void init(std::unordered_map<std::string, std::string> reqPathToFolderPathMap);

    std::unique_ptr<reply> handleRequest(const request &request_);

    bool url_decode(const std::string& in, std::string& out);
    
private:
    std::unordered_map<std::string, std::string> reqPathToFolderPathMap_;
    std::string base_;
};
#endif