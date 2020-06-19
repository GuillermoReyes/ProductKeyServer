#!/bin/bash
PATH_TO_BIN="./bin/webserver"
PATH_TO_TESTS="../tests"

#start webserver on port 8001
$PATH_TO_BIN ${PATH_TO_TESTS}/staticConfigLocal &
pid_server=$!
sleep .5
#integration test 1. Correct syntax
#curl --request POST -s -S localhost:8001 > ${PATH_TO_TESTS}/int_test_1_response.txt
(printf '%s\r\n%s\r\n%s\r\n\r\n'  \
    "GET /echo HTTP/1.1"                        \
    "Host: www.tutorialspoint.com"                 \
    "Connection: close"                     \
    | nc localhost 8001) > ${PATH_TO_TESTS}/int_test_1_response.txt
#cp ${PATH_TO_TESTS}/int_test_1_response.txt ${PATH_TO_TESTS}/int_test_1_expect_response.txt
diff ${PATH_TO_TESTS}/int_test_1_expect_response.txt ${PATH_TO_TESTS}/int_test_1_response.txt
if [[ $? -eq 0 ]]; then
    echo "integration test 1 success"; 
else 
    echo "integration test 1 failed"; 
    kill $pid_server
    exit 1;
fi



#integration test 2. Bad syntax
(printf '%s\r\n%s\r\n%s\r\n\r\n'  \
    "GET /nonexistHanlder HTTP/1.1"                        \
    "Host: www.tutorialspoint.com"                 \
    "Connection: close"                     \
    | nc localhost 8001) > ${PATH_TO_TESTS}/int_test_2_response.txt
diff ${PATH_TO_TESTS}/int_test_2_expect_response.txt ${PATH_TO_TESTS}/int_test_2_response.txt
if [[ $? -eq 0 ]]; then
    echo "integration test 2 success"; 
else 
    echo "integration test 2 failed"; 
    kill $pid_server
    exit 1;
fi

#integration test for reverse proxy handler
curl -s "https://www.ucla.edu/js/search.js" > proxy_expectation
curl -s "http://localhost:8001/ucla/js/search.js" > proxy_response
diff proxy_expectation proxy_response
if [ ! $? == 0 ]
then
  echo Test failed: Incorrect handling of proxy requests.
  exit 1
fi
rm proxy_expectation proxy_response

curl -s "https://www.ucla.edu/favicon.ico" > proxy_expectation
curl -s "http://localhost:8001/ucla/favicon.ico" > proxy_response
diff proxy_expectation proxy_response
if [ ! $? == 0 ]
then
  echo Test failed: Incorrect handling of proxy requests.
  exit 1
fi
rm proxy_expectation proxy_response



kill $pid_server
exit 0

#we might be supposed to use the local or external ip for netcat but not sure which one is right

