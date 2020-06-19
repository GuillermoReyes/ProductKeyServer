#include <iostream>

#include "HealthHandler.h"

std::unique_ptr<reply> HealthHandler::handleRequest(const request &request_) {
    std::unique_ptr<reply> reply_ = std::make_unique<reply>(); 
    reply_->status = reply::ok;
    return reply_;
}

