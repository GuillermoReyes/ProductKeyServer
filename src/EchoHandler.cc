#include <iostream>

#include "EchoHandler.h"

//for some reason when reply_.to_buffers() is called, only the content is being printed.
//The status and content type info is not being printed.




std::unique_ptr<reply> EchoHandler::handleRequest(const request &request_) {
    std::cout << "Echo handler being used" << std::endl; 
    std::unique_ptr<reply> reply_ = std::make_unique<reply>(); 
    reply_->status = reply::ok;
    reply_->headers.resize(2);
    reply_->content = request_.body;
    reply_->headers[0].name = "Content-Length";
    reply_->headers[0].value = std::to_string((reply_->content).length());
    reply_->headers[1].name = "Content-Type";
    reply_->headers[1].value = "text/plain";
    return reply_;
}

