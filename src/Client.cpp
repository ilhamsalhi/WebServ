#include "../include/Client.hpp"

Client::Client()
{
    headers = "";
    request_line_received = false;
    request_body_received = false;
    request_all_processed = false;
    start_responding = false;
    response_all_sent = false;
    keepalive_requests = 0;
    header_timeout = time(NULL);
    last_activity = header_timeout;
    header_buffer = NULL;
}

void Client::resetState()
{
    headers = "";
    request_line_received = false;
    request_body_received = false;
    request_all_processed = false;
    response_all_sent = false;
    start_responding = false;
    keepalive_requests += 1;
    last_activity = time(NULL);
    response.resetState();
    if (header_buffer != NULL)
        delete [] header_buffer;
    header_buffer = NULL;
}

void Client::setCookie(std::string &cookie)
{
    fields.insert(std::make_pair("Cookie", cookie));
}

void Client::setTransferEncoding(std::string &encoding)
{
    std::transform(encoding.begin(), encoding.end(), encoding.begin(), tolower);
    if (encoding == "chunked")
        fields.insert(std::make_pair("Transfer-Encoding", encoding));
    else
        throw RequestParsingException(STATUS_501);
    /*
        A server that receives a request message with a transfer encoding it
        does not understand SHOULD respond with 501 (Not Implemented).
    */
}

void Client::setContentType(std::string &content_type)
{
    std::map<std::string, std::string>::iterator it, ite;
    std::stringstream                            ss;
    std::string                                  boundary;
    size_t                                       pos;

    ss << content_type;
    getline(ss, content_type, ';');
    ss >> boundary;
    if ((pos = boundary.find("boundary=")) != std::string::npos)
    {
        ss.clear();
        ss << boundary.substr(pos + strlen("boundary="));
        getline(ss, fields["boundary"], ';');
    }
    std::transform(content_type.begin(), content_type.end(), content_type.begin(), tolower);
    fields.insert(std::make_pair("Content-Type", content_type));
}

void Client::setHost(std::string &host)
{
    std::vector<std::string> directives;
    std::stringstream        ss;
    std::string              host_uri, port;
    size_t                   colonpos;

    split(directives, host);
    if (directives.size() != 1)
        throw RequestParsingException(STATUS_400);
    colonpos = directives[0].find(':');
    if (colonpos != std::string::npos && colonpos == directives[0].length() - 1) // at least host_uri should be present
        throw RequestParsingException(STATUS_400);
    ss << directives[0];
    getline(ss, host_uri, ':');
    ss >> port;
    fields["Host"] = host;
    // optional
    // struct addrinfo				*addr;
    // if (!addr_resolver(&addr, host_uri.c_str(), port.c_str()))
    // 	throw RequestParsingException(STATUS_400);
    // freeaddrinfo(addr);
    /*
        syntax:
            Host = uri-host [ ":" port ]
        NB:
        A server MUST respond with a 400 (Bad Request) status code to any
        HTTP/1.1 request message that lacks a Host header field and to any
        request message that contains more than one Host header field or a
        Host header field with an invalid field-value.
    */
}

void Client::setContentLength(std::string &content_length)
{
    if (content_length.empty() ||
        find_if(content_length.begin(), content_length.end(), not_digit) != content_length.end())
        throw RequestParsingException(STATUS_400);
    fields.insert(std::make_pair("Content-Length", content_length));
    std::stringstream ss;
    ss << content_length;
    ss >> response.request_body_size;
    if (response.request_body_size > serverIt->client_max_body_size)
        throw RequestParsingException(STATUS_413);
    /*
     Content-Length = 1*DIGIT
    */
}

void Client::setConnection(std::string &connection)
{
    std::transform(connection.begin(), connection.end(), connection.begin(), tolower);
    if (connection == "keep-alive" || connection == "close")
        fields.insert(std::make_pair("Connection", connection));
    else
        throw RequestParsingException(STATUS_400);
}

void Client::setProtocolVersion(std::string &protocol_version) // HTTP-version = HTTP-name "/" DIGIT "." DIGIT
{
    std::stringstream ss;
    std::string       prefix, digit1, digit2;
    size_t            dotpos;

    if (protocol_version.find("HTTP/")) // start with "HTTP/"
        throw RequestParsingException(STATUS_400);
    dotpos = protocol_version.find('.', strlen("HTTP/"));
    if (dotpos != std::string::npos && dotpos == protocol_version.length() - 1)
        throw RequestParsingException(STATUS_400);
    ss << protocol_version;
    std::getline(ss, prefix, '/');
    std::getline(ss, digit1, '.');
    ss >> digit2;
    if (digit1.length() != 1 || !isdigit(digit1[0]) ||
        (!digit2.empty() && (digit2.length() > 1 || !isdigit(digit2[0]))))
        throw RequestParsingException(STATUS_400);
    if (protocol_version != "HTTP/1.1")
        throw RequestParsingException(STATUS_505);
    fields["protocol_version"] = protocol_version;
}

void Client::setMethod(std::string &method)
{
    std::vector<std::string>           methods;
    std::vector<std::string>::iterator it;
    const char *http_methods[] = {"GET", "POST", "DELETE", "HEAD", "PUT", "CONNECT", "OPTIONS", "Trace", "Patch"};

    methods.assign(http_methods, http_methods + sizeof(http_methods) / sizeof(const char *));
    if ((it = std::find(methods.begin(), methods.end(), method)) != methods.end() &&
        std::ptrdiff_t(it - methods.begin()) > 2)
        throw RequestParsingException(STATUS_501);
    else if (it == methods.end())
        throw RequestParsingException(STATUS_400);
    fields["method"] = method;
}

std::string Client::getMimeType(std::string &filepath)
{
    size_t                                       pos;
    std::string                                  extension;
    std::map<std::string, std::string>::iterator mimetypeIt;

    if ((pos = filepath.find_last_of('.')) != std::string::npos)
    {
        extension = filepath.substr(pos);
        mimetypeIt = Multiplexer::mime_types.find(extension);
        if (mimetypeIt != Multiplexer::mime_types.end())
            return mimetypeIt->second;
        return "application/octet-stream";
    }
    return "application/octet-stream";
}

std::string Client::getFileExtension(std::string &contentType)
{
    std::string                                  extension;
    std::map<std::string, std::string>::iterator mimetypeIt = Multiplexer::mime_types.begin();

    for (; mimetypeIt != Multiplexer::mime_types.end(); mimetypeIt++)
    {
        if (mimetypeIt->second == contentType)
            return mimetypeIt->first;
    }
    return ".txt";
}

std::string Client::generateFileName(std::string &contentType)
{
    std::time_t currentTime = std::time(NULL);
    std::tm    *localTime = std::localtime(&currentTime);

    // format the current time
    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(4) << 1900 + localTime->tm_year << "_";
    ss << std::setw(2) << 1 + localTime->tm_mon << "_";
    ss << std::setw(2) << localTime->tm_mday << "_";
    ss << std::setw(2) << localTime->tm_hour;
    ss << std::setw(2) << localTime->tm_min;
    ss << std::setw(2) << localTime->tm_sec << '_' <<  rand();

    return ("/" + ss.str() + getFileExtension(contentType));
}


void Client::setUri(std::string &uri)
{
    if (uri[0] != '/')
        throw RequestParsingException(STATUS_400);
    fields["request_target"] = uri;
}

void Client::getBuffer()
{
    std::streamsize read = CLIENT_HEADER_BUFFER_SIZE;
    if (header_buffer == NULL)
    {
        header_buffer = new char[read + 1];
        response.request_read = recv(connect_socket, header_buffer, read, 0);
        if (response.request_read >= 0)
        {
            last_activity = time(NULL);
            header_buffer[response.request_read] = '\0';
        }
    }
}
