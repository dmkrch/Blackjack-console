#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> //sockaddr, socklen_t
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>

#include <mutex>
#include "Exception.hpp"

#define INPUT_BUFFER_SIZE 100 //test: 100 bytes of buffer
#define DEFAULT_PORT 9034

class Server
{
public:
    Server();
    Server(int port);
    virtual ~Server();

    void init();
    void shutdown();
    int handleNewConnection();
    void closeConnection(int fd);

    std::string getReply(int fd);
    uint16_t sendMessage(int fd, const char *messageBuffer) const;
    uint16_t sendMessage(int fd, char *messageBuffer) const;
    uint16_t sendMessage(int fd, std::string messageBuffer) const;

private:
    //function prototypes
    void setup(int port);
    void initializeSocket();
    void bindSocket();
    void startListen();

private:
    std::mutex _sendMsgMutex;
    //fd_set file descriptor sets for use with FD_ macros
    fd_set _masterfds;
    fd_set _tempfds;

    //unsigned integer to keep track of maximum fd value, required for select()
    uint16_t _maxfd;

    //socket file descriptors
    int _mastersocket_fd; //master socket which receives new connections
    int _tempsocket_fd; //temporary socket file descriptor which holds new clients

    //client connection data
    struct sockaddr_storage _client_addr;
    //server socket details
    struct sockaddr_in _servaddr;
    //input buffer
    char _input_buffer[INPUT_BUFFER_SIZE];
};