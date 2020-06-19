#include "session.h"


session::session(boost::asio::io_service& io_service, NginxConfig config, HandlerDeterminer& HandlerDeterminer)
: socket_(io_service), socket_remote_(io_service)
{
    config_ = config;
    handlerDeterminer_ = &HandlerDeterminer;
}

tcp::socket& session::socket()
{
return socket_;
}


tcp::socket& session::socket_remote()
{
return socket_remote_;
}


void session::start()
{
socket_.async_read_some(boost::asio::buffer(data_, max_length),
    boost::bind(&session::handle_read, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void session::finishIncompleteRead(int charsReadAlready)
{
    std::cout << "finishing incomplete read" << std::endl; 
    
    socket_.async_read_some(boost::asio::buffer(data_ + charsReadAlready, max_length - charsReadAlready),
    boost::bind(&session::handle_read, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void session::handle_read(const boost::system::error_code& error,
    size_t bytes_transferred)
{
    std::cout << "Handle read" << std::endl; 
if (!error)
{
    // Do not close the socket if the request has not terminated with \r\n\r\n
    // Keep reading data until the request ends with that
    int dataLen = strlen(data_);

    if (!(data_[dataLen - 4] == '\r' && data_[dataLen - 3] == '\n' && data_[dataLen - 2] == '\r' && data_[dataLen - 1] == '\n'))
    {
        request_parser::result_type result;
        std::tie(result, std::ignore) = request_parser_.parse(
            request_, data_, data_ + bytes_transferred);

      /*std::string buffercont = request_.toString(); 
        std::string line;
        std::istringstream str(buffercont);


        // parsing the headers
        while (getline(str, line, '\n')) {
            if (line.empty() || line == "\r") {
                break; // end of headers reached
        }
            if (line.back() == '\r') {
                line.resize(line.size()-1);
        }
        // simply ignoring headers for now
        std::cout << "Ignore header: " << std::quoted(line) << "\n";
        }
        size_t contentsize = std::stoi(request_.headerMap["Content-Length"]);
        //std::string const body(std::istreambuf_iterator<char>{str}, request_.);
        std::string const body(str.tellg(), contentsize );
        std::cout << "Parsed content: " << std::quoted(body) << "\n";

        */
        if(result != request_parser::result_type::good  ) { finishIncompleteRead(dataLen); return;}

        if(request_.method == "POST") {std::cout << "POST method detected" << std::endl;}
        if((result == request_parser::result_type::good) && (request_.method == "POST")) std::cout << "Finished receiveing POST" << std::endl; 

        
        //return;
        
        }

    //set the bytes transfered to the total bytes transferred
    bytes_transferred = dataLen;



    std::cout <<"Parsing data" <<std::endl;



    //request handling
    request_parser::result_type result;
        std::tie(result, std::ignore) = request_parser_.parse(
            request_, data_, data_ + bytes_transferred);
    
    
    std::string buffercont = request_.toString(); 
    std::string line;
    std::istringstream str(buffercont);


    // parsing the headers
    while (getline(str, line, '\n')) {
        if (line.empty() || line == "\r") {
            break; // end of headers reached
        }
        if (line.back() == '\r') {
            line.resize(line.size()-1);
        }
        // simply ignoring headers for now
        std::cout << "Ignore header: " << std::quoted(line) << "\n";
    }
    std::cout << "User data should be here " << std::endl; 
    //std::string const body(str.tellg(), contentsize);
    std::string const body(std::istreambuf_iterator<char>{str}, {});
    std::cout << "Parsed content: " << std::quoted(body) << "\n";

    std::cout << "Data "  << "\n";

    std::cout << std::endl << data_ << std::endl; 
    std::cout << request_.uri << std::endl;

    if (result == request_parser::result_type::good  ) {
        std::cout << "NORMAL BEHAVE" << std::endl;
        //determine which handler to use
        RequestHandler* requestHandler = nullptr;
        std::string handlerName = handlerDeterminer_->pickHandler(request_, requestHandler);
        //check for error/invalud handler request
        if (handlerName == "") {
            //invalid handler request
            reply_ = reply::stock_reply(reply::bad_request);
        }
        else {
            //use the handler returned to handle the request
            reply_ = requestHandler->handleRequest(request_);
        }
    }
    else {
        if(result == request_parser::result_type::bad) std::cout <<"BAd BAD PACKET " << std::endl; 
        reply_ = reply::stock_reply(reply::bad_request);
    }
    std::cout << "Writing to the client" << std::endl; 
    //now we can write the reply back to the client
    boost::asio::async_write(socket_,
        //boost::asio::buffer(data_, bytes_transferred),
        reply_->to_buffers(),
        boost::bind(&session::handle_write, this,
        boost::asio::placeholders::error));
}
else
{
    delete this;
}
}

void session::handle_write(const boost::system::error_code& error)
{
if (!error)
{
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        boost::bind(&session::handle_read, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));

        //Added a shutdown command at the end of the session so that the socket doesnt hang
        //might have to change this for multithreading?
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
}
else
{
    delete this;
}
}
