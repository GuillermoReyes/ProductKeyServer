#include "ReverseProxyHandler.h"
using namespace std;

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <regex>
#include "header.h"
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <boost/log/trivial.hpp>

using boost::asio::ip::tcp;


// get host name and remove appending part
string ReverseProxyHandler::getHostName(string uri) {
  if (uri.substr(0,7) == "http://")
    uri.erase(0,7);
  else if (uri.substr(0,8) == "https://")
    uri.erase(0,8);
  for (auto it = uri.cbegin(); it != uri.cend(); ++it) {
    if (*it == '/') {
      uri.erase(it, uri.end());
      return uri;
    }
	}

  return uri;
}


unique_ptr<reply> ReverseProxyHandler::handleRequest(const request &request_) {
  //cout << "request uri: " << request_.uri << endl;
  vector<string> paths = SeparatePath(request_.uri);
  string host = getHostName(mapReverseProxyPath[paths[0]]);
  string path = "/";
  if (paths.size() == 2)
    path = paths[1];
  //cout << "path: " << path << endl;
  //cout << "host: " << host << endl;

  prefix = paths[0];
  string dest = host;

  int rand_num = rand() % 1000 + 1;

	// Construct URI sent to destination 
	string request_path = request_.uri;
	string uri_to_dest = dest + path;
  //cout << uri_to_dest << endl;
	if(uri_to_dest[uri_to_dest.size()-1]=='/') {
		uri_to_dest = uri_to_dest.substr(0,uri_to_dest.size()-1);
	}
	//BOOST_LOG_TRIVIAL(info) << "path:\n" << path;
	//BOOST_LOG_TRIVIAL(info) << "uri_to_dest:\n" << uri_to_dest;

	// CURL, get destinations' response, save headers and body into different local files.
  string body_file = "temp_file";
	body_file += to_string(rand_num);
	//BOOST_LOG_TRIVIAL(info) << "File name:\n" << body_file;
	string header_file = "curl_headers" + to_string(rand_num);
	string curl_get_headers = "curl -L -s --output " + header_file + " --head \"" + uri_to_dest + "\"";
	string curl_get_body = "curl -L -s --output " + body_file + " \"" + uri_to_dest + "\"";
	system(curl_get_headers.c_str());
  system(curl_get_body.c_str());
	
	// Read saved header and body 
	ifstream h(header_file, ios::in);
	string headers;
	char c;
	while (h.get(c)) headers += c;
	h.close();
	
	ifstream b(body_file, ios::in | ios::binary);
	string body;
	while (b.get(c)) body += c;
	b.close();

	// Construct reply to client 
  unique_ptr<reply> reply_ = make_unique<reply>(); 
	reply_->status = getStatusCode(headers);
  reply_->headers.resize(2);
  reply_->headers[0].name = "Content-Length";
  reply_->headers[1].name = "Content-Type";
  reply_->headers[1].value = getContentType(headers);
  if (reply_->headers[1].value.find("html") != string::npos)
    reply_->content = modifyContent(body, paths[0]);
  else
    reply_->content = body;
    reply_->headers[0].value = to_string(reply_->content.length());

	// Remove temporary headers and body files
  remove(header_file.c_str());
  remove(body_file.c_str());
	return reply_;
}


string ReverseProxyHandler::modifyContent(string content, string prefix) {
  prefix = prefix.substr(1);
  prefix += "/";
  // int c = 0;
  regex re_href("(href|src)=\"\/([^\"\']*)\"");
  smatch match;
  string out;
  while (regex_search(content, match, re_href)) {
    out += match.prefix();
    //cout << "FULLMATCH: " << match[0].str() << endl;
    //cout << "SUBMATCH: " << match[2].str() << endl;
    string full_match = match[0].str();
    string sub_match = match[2].str();
    if (sub_match.substr(0,7) == "http://" || sub_match.substr(0,8) == "https://") {
      out += full_match;
    } else {
      // c++;
      string replaced_match = full_match.replace(sub_match == "" ? full_match.find("/") + 1 : full_match.find(sub_match), sub_match.size(), prefix + sub_match);
      //cout << "MODIFIEDMATCH: " << replaced_match << endl << endl;
      out += replaced_match;
    }
    
    content = match.suffix();
  }
  // cout << "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP" << c << "PPPPPPPPPPPPPPPPPPPPP\n";
  return out;
}

string ReverseProxyHandler::getContentType(string headers) {
	int pos = headers.find("Content-Type: ");
  if (pos == string::npos)
    pos = headers.find("content-type: ");
	int endpos = pos + 14;
	int end   = headers.find("\r\n", pos);
	string content = headers.substr(endpos, end - endpos);
	return content;
}

vector<string> ReverseProxyHandler::SeparatePath(string path) {
  vector<string> paths;
  if (path.empty() || path[0] != '/'
      || path.find("..") != string::npos)
    return paths;
  int substr_start_index = 0;
	int counter = 0;
  for (size_t i = 1; i < path.length(); i++)
  {
		counter++;
    if (path[i] == '/')
    {
      paths.push_back(path.substr(substr_start_index,counter));
			substr_start_index = i;
			counter = 0;
      break;
    }
  }
  paths.push_back(path.substr(substr_start_index, path.length()));
	/*
	for (size_t i = 0; i < paths.size(); i++)
	{
		cout << paths.at(i) << endl;
	}
	*/	
  return paths;
}

http::server::reply::status_type ReverseProxyHandler::getStatusCode(string headers) {
  int start = headers.rfind("HTTP/1.1 ");
  int end   = headers.find("\r\n", start);
  //BOOST_LOG_TRIVIAL(info) << "Start: " << start+1;
  //BOOST_LOG_TRIVIAL(info) << "Length: " << end - start - 1;
  //BOOST_LOG_TRIVIAL(info) << "Headers length: " << headers.length();
  string http_status = headers.substr(start+9, end - start - 9);
  //BOOST_LOG_TRIVIAL(info) << "http_status:\n" << http_status;
  if (http_status == "200 OK")
    return http::server::reply::ok;
  else if (http_status == "201 Created")
    return http::server::reply::created;
  else if (http_status == "202 Accepted")
    return http::server::reply::accepted;
  else if (http_status == "204 No Content")
    return http::server::reply::no_content;
  else if (http_status == "300 Multiple Choices")
    return http::server::reply::multiple_choices;
  else if (http_status == "301 Moved Permanently")
    return http::server::reply::moved_permanently;
  else if (http_status == "302 Moved Temporarily")
    return http::server::reply::moved_temporarily;
  else if (http_status == "304 Not Modified")
    return http::server::reply::not_modified;
  else if (http_status == "400 Bad Request")
    return http::server::reply::bad_request;
  else if (http_status == "401 Unauthorized")
    return http::server::reply::unauthorized;
  else if (http_status == "403 Forbidden")
    return http::server::reply::forbidden;
  else if (http_status == "404 Not Found")
    return http::server::reply::not_found;
  else if (http_status == "500 Internal Server Error")
    return http::server::reply::internal_server_error;
  else if (http_status == "501 Not Implemented")
    return http::server::reply::not_implemented;
  else if (http_status == "502 Bad Gateway")
    return http::server::reply::bad_gateway;
  else if (http_status == "503 Service Unavailable")
    return http::server::reply::service_unavailable;
  else
    return http::server::reply::internal_server_error;
}