#ifndef MULTIPLEXER_HPP
#define MULTIPLEXER_HPP

#include "define.hpp"
#include "Response.hpp"
#include "webserv.hpp"
#include "Server.hpp"
#include "Client.hpp"

class Multiplexer
{
    SERVVECT                &servers;
    CLIENTVECT               clients;
    int                      epfd;
    int                      num_events;
    struct epoll_event       events[MAX_EVENTS];
    std::vector<std::string> headers_fields;

  public:
    Multiplexer(SERVVECT &servers, char **env);
    ~Multiplexer();
    void		  registerServers();
    void		  registerClient(SERVIT &serverIt);
    void		  dropClient(CLIENTIT &clientIt);
    void		  connectionListener();
    void		  getClientRequest(CLIENTIT &clientIt);
    bool	    ConnectionTimedOut(CLIENTIT& clientIt);
    void		  parseRequestLine(CLIENTIT &clientIt);
    void		  parseRequestHeaders(CLIENTIT &clientIt);
    void		  reviewHeaders(CLIENTIT &clientIt);
    void		  setServerByHost(CLIENTIT &clientIt);
    void		  handleResponse(CLIENTIT &clientIt);
    void    	loadMimeTypes();
    void    	loadDefErrorPages();
    SERVIT  	findListenSocket(int socket, SERVVECT &sockets);
    CLIENTIT	findConnectSocket(int socket, CLIENTVECT &sockets);

    static char                    **env;
    static const char *fields[HEADERS_FIELDS_SIZE];
    static void (Client::*fields_setters[HEADERS_FIELDS_SIZE])(std::string &field);
    static const char                        *defErrorPagesStrings[NUM_DEF_ERROR];
    static std::map<std::string, std::string> mime_types;
    static std::map<int, std::string>         defErrorPages;
    static int                                connections;

};
#endif
 