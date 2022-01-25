#include "Client.hpp"


int Client::check(int exp, const char* msg)
{
    if (exp == SOCKETERROR)
    {
        perror(msg);
        exit(1);
    }

    return exp; 
}


void Client::init() {
    sockaddr_in _serverAddr;
    char recvBuffer[BUFSIZE];

    /* now we create socket for client to connect to server afterwards */
    check((_clientSocket = socket(AF_INET, SOCK_STREAM, 0)),
    "Failed to create socket");

    _serverAddr.sin_family = AF_INET;         // setting type of family to struct
    _serverAddr.sin_port = htons(SERVERPORT); // setting port number to struct

    /* setting ip address of server to struct */
    if(inet_pton(AF_INET, IP_ADDRESS, &_serverAddr.sin_addr) <= 0)
    {
        printf("\n inet_pton error occured\n");
        return;
    }

    /* now we can throw request of connecting to our server */
    check(connect(_clientSocket, (sockaddr*)&_serverAddr, sizeof(_serverAddr)), 
        "Connect failed");
}

std::string Client::getReply() {
    char buffer[BUFSIZE];
    int nbytesrecv = recv(_clientSocket, buffer, BUFSIZE, 0);
    buffer[nbytesrecv] = '\0';

    if (nbytesrecv <= 0) {
        //problem
        if (0 == nbytesrecv) {
            close(_clientSocket); //well then, bye bye.
            return "error";
        } 
        else {
            perror("client recv() failed");
        }
        close(_clientSocket); //close connection to client
        return "error";
    }
    return std::string(buffer);
}

int Client::sendMessage(const char *messageBuffer) {
    return send(_clientSocket, messageBuffer, strlen(messageBuffer), 0);
}