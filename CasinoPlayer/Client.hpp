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

#include <chrono>
#include <thread>

#define SERVERPORT 9034
#define IP_ADDRESS "127.0.0.1"
#define BUFSIZE 300
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
    void disconnect() { close(_clientSocket); }

private:
    void init();
    int check(int exp, const char* msg);

    void callDelay() {
        std::this_thread::sleep_for(std::chrono::milliseconds(consts::waitingSeconds * 1000));
    }


    int _clientSocket;
    sockaddr_in _serverAddr;
};