


#(echo "GET / HTTP/1.1\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nConnection: close\r\n\r\n" | nc -q 3 localhost 8001)


printf '%s\r\n%s\r\n%s\r\n\r\n'  \
    "GET /static1/helpfile.txt HTTP/1.1"                        \
    "Host: www.example.com"                 \
    "Connection: close"                     \
    | nc -q 3 localhost 8001     
