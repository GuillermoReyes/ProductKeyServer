#include "request.h"
#include <iostream>

namespace http {
namespace server {
std::string request::toString() const {

    std::string request_str;

    request_str.append(method);
    request_str.append(" ");
    request_str.append(uri);
    request_str.append(" ");

    switch (http_version_minor) {
        case 0:
            request_str.append("HTTP/1.0"); break;
        case 1:
            request_str.append("HTTP/1.1"); break;
        default:
            assert(0);
    }
    request_str.append("\r\n");

    for (http::server::header h : headers) {
        request_str.append(h.name);
        request_str.append(": ");
        request_str.append(h.value);
        request_str.append("\r\n");
    }
    request_str.append("\r\n");

    return request_str;
}

void request::fillMapFromHeaders() {
    body = this->toString();
    headerMap.clear();
    for(std::vector<header>::iterator it = headers.begin(); it != headers.end(); it++) {
        headerMap[it->name] = it->value;
    }
}


}}
