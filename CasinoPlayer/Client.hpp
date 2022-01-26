#pragma once

#include "Consts.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <cstring>

#define SERVERPORT 9034
#define IP_ADDRESS "127.0.0.1"
#define BUFSIZE 100
#define SOCKETERROR (-1)


class Client {
public:
    Client() {
        init();
    }

    std::string getReply();
    int sendMessage(const char *messageBuffer);
    int sendMessage(std::string messageBuffer);
    int getClientSocket() const { return _clientSocket; }

private:
    void init();
    int check(int exp, const char* msg);

    int _clientSocket;
    sockaddr_in _serverAddr;
};