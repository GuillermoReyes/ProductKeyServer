/* BOOST INCLUDES: https://www.boost.org/doc/libs/1_65_1/libs/log/doc/html/index.html */
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/signals2.hpp>

/* HEADERS */
#include "logger.h"
#include "request.h"

namespace r_log = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace r_log::trivial;
using boost::asio::ip::tcp;

/* DEFINE LOGGER CONSTRUCTOR */
Logger::Logger() {
    init();
    r_log::add_common_attributes();
    r_log::record rec = lg.open_record();
    r_log::add_console_log(std::cout, boost::log::keywords::format = ">> %Message%");}

/* DEFINE LOGGER INIT */
void Logger::init() {
    r_log::add_file_log
    (   keywords::file_name = "../log/SYSLOG_%N.log",
        keywords::rotation_size = 10 * 1024 * 1024,
        keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0),
        keywords::format = "[%TimeStamp%]:[%ThreadID%]:%Message%",
        keywords::auto_flush = true
    );
}
void Logger::log_server_init() {
    BOOST_LOG_SEV(lg, trace) << "Trace: " << "Server has been initialized";
}

void Logger::log_trace_write(std::string trace_msg) {
    BOOST_LOG_SEV(lg, trace) << "Trace: " << trace_msg;
}

void Logger::log_error_write(std::string error_msg){
    BOOST_LOG_SEV(lg, error) << "Error: " << error_msg;
}
void Logger::log_debug_write(std::string debug_msg){
    BOOST_LOG_SEV(lg, debug) << "Debug: " << debug_msg;
}

void Logger::log_warning_write(std::string warning_msg){
    BOOST_LOG_SEV(lg, warning) << "Warning: " << warning_msg;
}
void Logger::log_shutdown() {
    BOOST_LOG_SEV(Logger::lg, warning) << "Warning: "  << "Shutting down the server...";
}

void Logger::log_http_request(http::server::request http_request, tcp::socket& m_socket) {
    std::stringstream stream;
    stream << "Trace: ";
    stream << http_request.method << " " << http_request.uri
    << " HTTP " << http_request.http_version_major << "." << http_request.http_version_minor;
    stream << " Sender IP: " << m_socket.remote_endpoint().address().to_string();
    BOOST_LOG_SEV(lg, trace) << stream.str();
}

void Logger::log_request_metrics(http::server::request http_request, tcp::socket& m_socket, const std::string res_status) {
    std::stringstream stream;
    stream << "ResponseMetrics::";
    stream << http_request.method << " " << http_request.uri
    << " HTTP/" << http_request.http_version_major << "." << http_request.http_version_minor;
    stream << " IP: " << m_socket.remote_endpoint().address().to_string();
    stream << " Response Status: " << res_status;
    BOOST_LOG_SEV(lg, trace) << stream.str();
}

void Logger::log_trace_trivial(){ BOOST_LOG_TRIVIAL(trace) << "A trace severity message"; }
void Logger::log_debug_trivial(){ BOOST_LOG_TRIVIAL(debug) << "A debug severity message"; }
void Logger::log_warning_trivial(){ BOOST_LOG_TRIVIAL(warning) << "A warning severity message"; }
void Logger::log_error_trivial(){ BOOST_LOG_TRIVIAL(error) << "An error severity message"; }
void Logger::log_fatal_trivial(){ BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message"; }
Logger* Logger::logger = 0;

/* CITATIONS: UPE tutoring session & TA slides/discussion */
