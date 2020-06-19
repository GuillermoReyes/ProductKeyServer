#ifndef LOG_H
#define LOG_H
#include <boost/asio.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/signals2.hpp>
#include "request.h"
using boost::asio::ip::tcp;
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace logging::trivial;
class Logger {
    public:
    src::severity_logger<severity_level> lg;
    Logger();
    void init();
    void log_server_init();
    void log_trace_write(std::string info_message);
    void log_error_write(std::string error_message);
    void log_debug_write(std::string debug_message);
    void log_warning_write(std::string warning_message);
    void log_http_request(http::server::request http_request, tcp::socket& m_socket);
    void log_request_metrics(http::server::request http_request, tcp::socket& m_socket, const std::string res_status="200");
    void log_shutdown();

    static Logger * logger;
    static void log_trace_trivial();
    static void log_debug_trivial();
    static void log_warning_trivial();
    static void log_error_trivial();
    static void log_fatal_trivial();

    static Logger * getLogger() {
        if (Logger::logger==0) Logger::logger = new Logger();
        return Logger::logger;
    }
};
#endif

/* CITATIONS: header adapted from UPE tutoring session & TA slides/discussion */
