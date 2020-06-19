#include <iostream>


#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>

#include "KeyHandler.h"



std::unique_ptr<reply> KeyHandler::handleRequest(const request &request_) {
    std::unique_ptr<reply> reply_ = std::make_unique<reply>(); 
    std::cout << "KEY HANDLER " << std::endl;

    boost::filesystem::path p (".");

    boost::filesystem::directory_iterator end_itr;

    // cycle through the directory
    for (boost::filesystem::directory_iterator itr(p); itr != end_itr; ++itr)
    {
        // If it's not a directory, list it. If you want to list directories too, just remove this check.
        
            // assign current file name to current_file and echo it out to the console.
            std::string current_file = itr->path().string();
            std::cout << current_file << std::endl;
        
    }
    std::cout << request_.body << std::endl; 
    //std::cout << request_.toString() << std::endl; 
    std::cout << request_.method << std::endl; 

    //TEMPORARY IMPLEMENTATION
    reply_->status = reply::ok;
    std::cout << "abput to prepare response" << std::endl; 
    std::cout << "Request uri : " << request_.uri << std::endl; 
    std::vector<http::server::header> reqheaders = request_.headers;
    for(int i = 0; i < reqheaders.size(); i++){
        std::cout << reqheaders[i].name + ":" + reqheaders[i].value << std::endl; 
    }
    std::map<std::string, std::string> hmap = request_.headerMap; 
    for(auto i : hmap){
        std::cout << i.first + ":" + i.second << std::endl;
    }
    std::cout << "MAde it this far" << std::endl; 
    
   /* std::cout << boost::filesystem::current_path().string() << std::endl; 
    boost::regex uri_parse("(?<==)([\\w]*)(?=|&|\\n)");
    std::string request_uri = request_.uri; 
    boost::smatch what;
    std::string::const_iterator start, end;
    start = request_uri.begin(); 
    end = request_uri.end(); 
    boost::match_flag_type flags = boost::match_default;
    std::vector<std::string> arguments; 
    while(regex_search(start, end, what, uri_parse, flags)){
        
        std::cout << "Parsed key value: " << std::string(what[1].first, what[1].second) << std::endl; 
        arguments.push_back(std::string(what[1].first, what[1].second));
        start = what[0].second; 
        flags |= boost::match_prev_avail; 
        flags |= boost::match_not_bob; 

    }
    */
    /*
    std::cout << "regex is fine" << std::endl; 
    
    //make an md5 hash 
    boost::property_tree::ptree keystructure;
    boost::property_tree::ptree keystructure2;
    
    keystructure.put("Name",arguments[0]);
    keystructure.put("Email", arguments[1]);
    keystructure.put("password", arguments[2]);
    
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::ostringstream strCout;
    std::cout.rdbuf( strCout.rdbuf() );
    write_xml(std::cout, keystructure);
    //std::cout.flush(); 
    std::cout.rdbuf(oldCoutStreamBuf);
    std::string content = strCout.str(); 
    std::cout << content <<std::endl; 

    std::ifstream ifs("./content/template.html", std::ios::in);
    std::string acc;
    char c; 
    while (ifs.get(c)) acc += c;
    ifs.close();

    content = acc + "<p hidden id=\"params\"> \n" + content  +"</p> " ;
    */
    std::string request_uri; 
    request_uri = request_.uri;
    if(request_uri[0] == '/'){
      request_uri.erase (0,1);
    }
    std::ifstream f(request_uri.c_str(), std::ios::in | std::ios::binary);
    if( !f) {
      std::cout << " File not found! " <<std::endl;  
      reply_ = http::server::reply::stock_reply(reply::not_found);
      return reply_; 

    }
    std::string content;
    char c;
    while (f.get(c)) content += c;
    f.close(); 
    std::string extension; 
    std::string request_path = boost::filesystem::current_path().string() + request_.uri; 
    size_t cursor = request_path.find_last_of(".");
    if(cursor != std::string::npos)
      extension = request_path.substr(cursor + 1);
    reply_->status = reply::ok;
    reply_->content = content;
    reply_->headers.resize(2);
    reply_->headers[0].name = "Content-Length";
    reply_->headers[0].value = std::to_string(reply_->content.size());
    reply_->headers[1].name = "Content-Type";
    reply_->headers[1].value = "text/html";
    std::cout << "finished packet for jey handler response " << std::endl; 
    
    //fs.open ("content/html_result.html", std::fstream::in | std::fstream::out | std::fstream::app);
    
    
    







    





    //TEMPLATE FOR CORRECT IMPLEMENTATION    

    //BASED ON THE URI (request_.uri)
    //Either
        //generate a new key from a hash based on the request information
        //activate a key that has been generated
        //validate if a key exists
        //validate if a key has been activated
    

    // if (request_.uri == "some_keygen_request") {
            //generate key then fill out reply with informative response
    // }
    // else if (request_.uri == "some_other_request") {
    // }
    // etc...
    std::cout << "reply sent from keyhandler" << std::endl; 
    return reply_;
}

bool KeyHandler::doesKeyExist(std::string& key) {
    if (keyToActivationMap.find(key) == keyToActivationMap.end())
    {
        return false;
    }
    else
    {
        return true;
    }
    
}


int KeyHandler::keyActivated(std::string& key) {
    std::map<std::string, bool>::iterator it = keyToActivationMap.begin();
    it = keyToActivationMap.find(key);
    if (it == keyToActivationMap.end())
    {
        //Key not in map
        return -1;
    }
    else if (it->second == false) {
        //key in map but not yet activated
        return 0;
    }
    else
    {
        //key in map and is activated
        return 1;
    }
}


int KeyHandler::activateKey(std::string& key) {
    std::map<std::string, bool>::iterator it = keyToActivationMap.begin();
    it = keyToActivationMap.find(key);
    if (it == keyToActivationMap.end())
    {
        //Key not in map
        //so return error msg
        return -1;
    }
    else if (it->second == false) {
        //key in map but not yet activated
        //so activate key
        it->second = true;
        return 0;
    }
    else
    {
        //key in map and is activated already
        //return that informatin
        return 1;
    }
}

void KeyHandler::createKeyAndAddToMap(const request& request) {
    //create key from hashing provided info
        // key = hash(string_from_request)

    //if the key has already been created then add random chars to string_from_request and try hashing again
        // if (keyExists(key)) {
        //     //modify string from request and try hashing again
        // }


    //if new key is not already in map then add it to the map
        // keyToActivationMap[key] = false;
    
}