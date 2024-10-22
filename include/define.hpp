#ifndef DEFINE_HPP
#define DEFINE_HPP

#define SERVER        "webserv"
#define PROTOCOL_VERS "HTTP/1.1 "
#define URI           "request_target"
#define NUM_DEF_ERROR 41
#define CRLF          "\r\n"

/*                  server default configuration                */
#define DEFAULT_CONF_PATH            "config/default.conf"
#define DEFAULT_HOST                 "localhost"
#define DEFAULT_PORT                 "1337"
#define DEFAULT_ROOT                 "./www"
#define DEFAULT_AUTOINDEX            false
#define DEFAULT_CLIENT_MAX_BODY_SIZE 1048576 // 1M
#define MAX_REQUEST_SIZE             107374182400 //  10GB --> 107374182400B // 5GB --> 5368709120 // 1GB --> 1073741824 // 9223372036GB --> MAX LONG LONG 9223372036854775807


#define KB                           1024
#define MB                           1048576
#define GB                           1073741824

/*                  ConfigParser                                */
#define NUM_SERV_DIREC  8
#define NUM_LOCAT_DIREC 7

/*                  Multiplexer                                  */

#ifdef KEEPALIVE
    # define KEEPALIVE_CONN 1
#else
    # define KEEPALIVE_CONN 0
#endif

#define Running                     true
#define HEADERS_FIELDS_SIZE         6
#define MAX_EVENTS                  1024
#define BUFFER_SIZE                 1024
#define CLIENT_HEADER_BUFFER_SIZE   1024
#define CLIENT_RESPONSE_BUFFER_SIZE 1024
#define CLIENT_SEND_TIMEOUT         30
#define BACKLOG                     100
#define CONNECTIONS                 400
#define MIMETYPE_PATH               "./config/mimes"

/*              Response                */
#define CGI_TIMEOUT                 4

/*                       status codes                           */
//====== 1XX =====//
#define STATUS_100 "100 Continue"
#define STATUS_101 "101 Switching Protocols"
//====== 2XX =====//
#define STATUS_200 "200 OK"
#define STATUS_201 "201 Created"
#define STATUS_202 "202 Accepted"
#define STATUS_203 "203 Non-Authoritative Information"
#define STATUS_204 "204 No Content"
#define STATUS_205 "205 Reset Content"
#define STATUS_206 "206 Partial Content"
//====== 3XX =====//
#define STATUS_300 "300 Multiple Choices"
#define STATUS_301 "301 Moved Permanently"
#define STATUS_302 "302 Found"
#define STATUS_303 "303 See Other"
#define STATUS_304 "304 Not Modified"
#define STATUS_305 "305 Use Proxy"
#define STATUS_306 ""
#define STATUS_307 "307 Temporary Redirect"
//====== 4XX =====//
#define STATUS_400 "400 Bad Request"
#define STATUS_401 "401 Unauthorized"
#define STATUS_402 "402 Payment Required"
#define STATUS_403 "403 Forbidden"
#define STATUS_404 "404 Not Found"
#define STATUS_405 "405 Method Not Allowed"
#define STATUS_406 "406 Not Acceptable"
#define STATUS_407 "407 Proxy Authentication Required"
#define STATUS_408 "408 Request Timeout"
#define STATUS_409 "409 Conflict"
#define STATUS_410 "410 Gone "
#define STATUS_411 "411 Length Required"
#define STATUS_412 "412 Precondition Failed"
#define STATUS_413 "413 Request Entity Too Large"
#define STATUS_414 "414 Request-URI Too Long"
#define STATUS_415 "415 Unsupported Media Type"
#define STATUS_416 "416 Requested Range Not Satisfiable"
#define STATUS_417 "417 Expectation Failed"
//====== 5XX =====//
#define STATUS_500 "500 Internal Server Error"
#define STATUS_501 "501 Not Implemented"
#define STATUS_502 "502 Bad Gateway"
#define STATUS_503 "503 Service Unavailable"
#define STATUS_504 "504 Gateway Timeout"
#define STATUS_505 "505 HTTP Version Not Supported"

#define IS_FILE 0
#define IS_DIRECTORY 1

/*          return status                */
#define IS_FILE 0
#define IS_DIRECTORY 1
#define ERROR -1

/*           cgi env indexes           */
#define METHOD 0
#define CONTENT_LENGTH 1
#define CONTENT_TYPE 2


#define GET 0
#define POST 1

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define ORANGE  "\033[38;5;208m"
#define BLUE    "\033[34m"
#endif
