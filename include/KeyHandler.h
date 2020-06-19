#ifndef KEYHANDLER
#define KEYHANDLER
#include "RequestHandler.h"
#include <map>

#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <vector>
#include <fstream>
#include <string>

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <boost/uuid/sha1.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/detail/xml_parser_write.hpp>
#include <boost/property_tree/detail/xml_parser_writer_settings.hpp>
#include <boost/property_tree/stream_translator.hpp>
#include <streambuf>
#include <locale>

class KeyHandler : public RequestHandler {
public:   
    std::unique_ptr<reply> handleRequest(const request &request_);

    static RequestHandler* Init(const NginxConfig& config) {
        //return a new echohandler
        return new KeyHandler();
    }

    
    bool doesKeyExist(std::string& key);
    int keyActivated(std::string& key);
    int activateKey(std::string& key);
    void createKeyAndAddToMap(const request& request);

    std::map<std::string, bool> keyToActivationMap;
    std::map<std::string, std::string> md5toIDKMap; 

    private:
    
};



#endif