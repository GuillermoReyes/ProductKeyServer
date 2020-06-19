#include "StatusHandler.h"


#include "session.h"

StatusHandler::StatusHandler(){
    
}
std::unique_ptr<reply> StatusHandler::handleRequest(const request &request_){
    std::unique_ptr<reply> reply_ = std::make_unique<reply>();
    reply_->status = reply::ok;
    session::num_requests ++;
    
    session::request_received_[request_.uri].push_back(reply_->status);
    
    std::string req_inf = "";
    std::string handler_inf = "";
    std::string html_content = "";
    for (auto it = session::request_received_.begin(); it != session::request_received_.end(); ++it){
        for(int j = 0; j <  (it->second).size(); j++){
            req_inf += "<tr><td>" + it->first +"</td><td>" + std::to_string((it->second)[j]) + "</td></tr>";
            //session::handlerDeterminer_->getHandlerNameFromReqPath("ggg");
            std::cout << "test" << std::endl;
            
        }
    }
    
    //Get the handler info from
    /*for (auto it = HandlerDeterminer::handlers->begin(); it != HandlerDeterminer::handlers->end(); ++it){
     
     handler_inf += "<tr><td>" it->first + "<td></td>" + it->second + "</td></tr>";
     std::cout << it->first << std::endl;
     } */
    
    for(auto it = HandlerDeterminer::handlersAvaliable.begin(); it != HandlerDeterminer::handlersAvaliable.end(); ++it){
        handler_inf += "<tr><td>" + it->first + "</td><td>" + it->second + "</td></tr>";
    }
    
    //HTML content in reply
    std::string display_html_content = "<html><head>""<title>Server Status</title></head>"
    "<body><h2>Total number of requests</h1><div>"+ std::to_string(session::num_requests) +"</div>"
    "<h2>Detail Request Status</h2>"
    "<table>"
    "<tr><th>URL Requested</th><th>Return Code</th></tr>" + req_inf + "</table>"
    "<h2>Request Handlers</h2>"
    "<table>"
    "<tr><th>URL Prefix</th><th>Handler</th></tr>" + handler_inf +  "</table>"
    "</body>"
    "</html>";
    
    
    reply_->headers.resize(2);
    reply_->content = display_html_content;
    reply_->headers[0].name = "Content-Length";
    reply_->headers[0].value = std::to_string((reply_->content).length());
    reply_->headers[1].name = "Content-Type";
    reply_->headers[1].value = "text/html";
    
    return reply_;
    
}


