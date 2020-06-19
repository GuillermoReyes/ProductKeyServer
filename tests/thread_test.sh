#!/bin/bash
PATH_TO_BIN="./bin/webserver"
PATH_TO_TESTS="../tests"

#start webserver on port 8001
$PATH_TO_BIN ${PATH_TO_TESTS}/staticConfigLocal &
pid_server=$!
sleep .5
#thread test 1 -- overlapping requests
#curl --request POST -s -S localhost:8001 > ${PATH_TO_TESTS}/int_test_1_response.txt
(printf '%s\r\n%s\r\n%s\r\n\r\n'  \
    "GET /echo HTTP/1.1"                        \
    "Host: FIRST REQUEST"                 \
    "Connection: close"                     \
    | nc -i 1 localhost 8001) &

sleep .5
SECOND_REQUEST_COMPLETED=$(printf '%s\r\n%s\r\n%s\r\n\r\n'  \
    "GET /echo HTTP/1.1"                        \
    "Host: SECOND REQUEST"                 \
    "Connection: close"                     \
    | nc localhost 8001)


# echo "FIRST RESPONE:"
# echo $RESPONSE_1
# echo "SECOND REQUEST"
# echo $SECOND_REQUEST_COMPLETED
# echo "FINISHED"
if [[ $SECOND_REQUEST_COMPLETED != "" ]]; then
    echo "thread test 1 success"; 
else 
    echo "thread test 1 failed"; 
    kill $pid_server
    exit 1;
fi

kill $pid_server
exit 0

