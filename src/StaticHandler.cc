#include "StaticHandler.h"
#include <fstream>
#include <sstream>
#include <string>
#include "reply.h"
#include "request.h"


//for some reason when reply_.to_buffers() is called, only the content is being printed.
//The status and content type info is not being printed.

    // void StaticHandler::init(std::unordered_map<std::string, std::string> reqPathToFolderPathMap) {
    //   reqPathToFolderPathMap_ = reqPathToFolderPathMap;
    // }
    /* RequestHandler* StaticHandler::Init(const NginxConfig& config){
        root = "/";
    for (int i = 0; i < config.statements_.size(); ++i) {
        if (config.statements_[i]->tokens_[0] == "root" &&
                config.statements_[i]->tokens_.size() == 2) {
            root = config.statements_[i]->tokens_[1];
        }
    }
     }
*/
    std::unique_ptr<reply> StaticHandler::handleRequest(const request &request_) {
    //modify reply based on the request and the base
    // Decode url to path.
   for (auto i: request_.headers)
      std::cout << "Key:" << i.name << "  Value:" << i.value <<std::endl;
    
    
    std::cout << "Static Handler being used " << std::endl; 
    std::cout << "Current Dir contents" << std::endl; 
    boost::filesystem::path p (".");
    
    boost::filesystem::directory_iterator end_itr;

    // cycle through the directory
    for (boost::filesystem::directory_iterator itr(p); itr != end_itr; ++itr)
    {
        // If it's not a directory, list it. If you want to list directories too, just remove this check.
        //if (boost::filesystem::is_regular_file(itr->path())) {
            // assign current file name to current_file and echo it out to the console.
            std::string current_file = itr->path().string();
            std::cout << current_file << std::endl;
        //}
    }
    std::string request_uri = request_.uri; 
    std::cout << " Root Path " << base_ << std::endl; 
    std::cout <<request_.uri << std::endl; 
    std::cout << "Current Path: " << boost::filesystem::current_path().string() << std::endl; 
    std::string request_path = boost::filesystem::current_path().string() + request_.uri; 
    std::cout << "Request path : " << request_path << std::endl; 
    std::unique_ptr<reply> reply_ = std::make_unique<reply>(); 
    //reply_->mime_type = request_.url_extension; 

    //std::ifstream file(request_path.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
    std::cout << boost::filesystem::current_path() << std::endl; 
    //std::cout << "Root Path : " << boost::filesystem::root_path() << std::endl; 
    if(request_uri[0] == '/'){
      request_uri.erase (0,1);
    }
    std::cout << "Request string is " << request_uri << std::endl; 
    boost::filesystem::path boost_path(request_uri);
    if(!boost::filesystem::exists(boost_path) || !boost::filesystem::is_regular_file(boost_path)){
      reply_ = http::server::reply::stock_reply(reply::not_found);
      std::cout << "invalid request path " <<std::endl; 
      return reply_;
    }
    std::string pathtoopen = "html/index.html";
    std::ifstream f(request_uri.c_str(), std::ios::in | std::ios::binary);
    if( !f) {
      std::cout << " File not found! " <<std::endl;  
      reply_ = http::server::reply::stock_reply(reply::not_found);
      return reply_; 

    }
    /*if (!url_decode(request_.uri, request_path))
    {
      reply_ = http::server::reply::stock_reply(reply::bad_request);
      return reply_; 
    }
    */
    // Request path must be absolute and not contain "..".
    /*if (request_path.empty() || request_path[0] != '/'
        || request_path.find("..") != std::string::npos)
    {
      reply_ = http::server::reply::stock_reply(reply::bad_request);
      return reply_;
    }
    */
  // If path ends in slash (i.e. is a directory) then add "index.html".
   /* if (request_path[request_path.size() - 1] == '/')
    {
      request_path += "index.html";
    }
    */
    // Determine the file extension.
    /*std::size_t last_slash_pos = request_path.find_last_of("/");
    std::size_t last_dot_pos = request_path.find_last_of(".");
    std::string extension;
    if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
    {
      extension = request_path.substr(last_dot_pos + 1);
    }
    */
    // Open the file to send back.
    /*std::string full_path = base_ + request_path;
    std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
    if (!is)
    {
      reply_ = http::server::reply::stock_reply(reply::not_found);
      return reply_;
    }
    */
    // Fill out the reply to be sent to the client.
    std::string content;
    char c;
    while (f.get(c)) content += c;
    f.close(); 
    std::string extension; 
    size_t cursor = request_path.find_last_of(".");
    if(cursor != std::string::npos)
      extension = request_path.substr(cursor + 1);
    reply_->status = reply::ok;
    
    
    reply_->content = content;
    reply_->headers.resize(2);
    reply_->headers[0].name = "Content-Length";
    reply_->headers[0].value = std::to_string(reply_->content.size());
    reply_->headers[1].name = "Content-Type";
    reply_->headers[1].value = http::server::mime_types::extension_to_type(extension);
    std::cout << "reply sent" << std::endl; 
    return reply_; 
}
bool StaticHandler::url_decode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}
