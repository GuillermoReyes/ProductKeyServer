cmake_minimum_required(VERSION 3.10.0)
project(gkhd_web)

# Make sure developers do not run cmake in the main project directory, to keep
# build artifacts from becoming clutter
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed.
    Please make a new directory (called a build directory) and run CMake from there.
    You may need to remove CMakeCache.txt." )
endif()

# Turn on debug builds if we are building in a devel shell
if (CMAKE_BUILD_TYPE STREQUAL "" AND "$ENV{DEVEL_SHELL}" STREQUAL "1")
    message(STATUS "Setting debug build type by default in devel shell")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Output binaries to a sub directory "bin"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable cmake testing
include(CTest)
enable_testing()

# Enable GoogleTest
include(GoogleTest)
add_subdirectory(/usr/src/googletest googletest)


# Enable Boost
# Use static libraries so binaries can be deployed without a full boost install
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.50 REQUIRED COMPONENTS system filesystem regex log_setup log )
message(STATUS "Boost version: ${Boost_VERSION}")
include_directories(include)
# TODO(!): Update name and srcs
# add_library(example_lib src/example_lib.cc)
#add_library(config_parser_lib src/config_parser.cc)
# TODO(!): Update executable name, srcs, and deps
# add_executable(example_server src/example_server_main.cc)
add_library(config_parser src/config_parser.cc)
add_library(server src/server.cc src/session.cc)
add_library(session src/session.cc src/server.cc src/reply.cc)
add_library(request_parser src/request_parser src/request.cc)
add_library(reply src/reply.cc)
add_library(request_handler src/EchoHandler.cc src/StaticHandler.cc src/ReverseProxyHandler.cc src/HealthHandler.cc src/KeyHandler.cc src/mime_types src/config_parser.cc src/reply.cc )
add_library(handler_determiner src/HandlerDeterminer.cc src/EchoHandler.cc src/StaticHandler.cc src/ReverseProxyHandler.cc src/HealthHandler.cc src/KeyHandler.cc src/config_parser.cc src/request.cc src/reply.cc src/mime_types.cc)
add_library(logger src/logger.cc)
add_library(request src/request.cc)

add_executable(webserver src/server_main.cc)
# target_link_libraries(example_server example_lib Boost::system)
target_link_libraries(webserver Boost::system config_parser server session request_handler request_parser logger
                reply handler_determiner request Boost::filesystem Boost::regex Boost::log_setup Boost::log Boost::thread )
# TODO(!): Update test executable name, srcs, and deps
# add_executable(example_lib_test tests/example_lib_test.cc)
add_executable(config_parser_test tests/config_parser_test.cc)
add_executable(request_parser_test tests/request_parser_test.cc)
add_executable(reply_test tests/reply_test.cc)
add_executable(server_test tests/server_test.cc)
add_executable(session_test tests/session_test.cc)
add_executable(handler_determiner_test tests/handler_determiner_test.cc)
add_executable(logger_test tests/logger_test.cc)
add_executable(health_handler_test tests/health_handler_test.cc)
add_executable(key_handler_test tests/key_handler_test.cc)
add_executable(reverse_proxy_handler_test tests/reverse_proxy_handler_test.cc)
# # target_link_libraries(example_lib_test example_lib gtest_main)
target_link_libraries(config_parser_test config_parser gtest_main Boost::regex)
target_link_libraries(request_parser_test request_parser gtest_main)
target_link_libraries(reply_test reply gtest_main Boost::system)
target_link_libraries(server_test reply server session request_handler request_parser handler_determiner
                    gtest_main gmock_main Boost::system
                    Boost::filesystem Boost::regex Boost::log_setup Boost::log)
target_link_libraries(session_test session request_handler request_parser handler_determiner
                gtest_main gmock_main logger Boost::system
                Boost::filesystem Boost::regex Boost::log_setup Boost::log)
target_link_libraries(handler_determiner_test handler_determiner gtest_main Boost::system Boost::thread Boost::log)
target_link_libraries(logger_test logger gtest_main Boost::log)
target_link_libraries(health_handler_test gtest_main Boost::log request_handler)
target_link_libraries(reverse_proxy_handler_test gtest_main Boost::log request_handler)
target_link_libraries(key_handler_test gtest_main Boost::log request_handler )
# # TODO(!): Update with test binary
# # gtest_discover_tests(example_lib_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(config_parser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(request_parser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(reply_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(server_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(session_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(handler_determiner_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(logger_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(health_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(reverse_proxy_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(key_handler_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
add_test(NAME integration_test COMMAND ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/tests/integration_test.sh)
add_test(NAME thread_test COMMAND ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/tests/thread_test.sh)
# # TODO(!): Update with target/test targets
include(cmake/CodeCoverageReportConfig.cmake)
# #generate_coverage_report(TARGETS session server config_parser  TESTS config_parser_test)
generate_coverage_report(TARGETS webserver session server config_parser request_parser logger
                         reply handler_determiner TESTS config_parser_test request_parser_test
                         reply_test session_test server_test handler_determiner_test logger_test health_handler_test reverse_proxy_handler_test key_handler_test)

