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
}

void Client::run() { 
    char buffer[BUFSIZE];

    /* now we can throw request of connecting to our server */
    check(connect(_clientSocket, (sockaddr*)&_serverAddr, sizeof(_serverAddr)), 
        "Connect failed");

    std::string name;
    std::string bet;


    // in that reply, server will ask us to write his name
    std::cout << getReply(_clientSocket);
    std::cin >> name;
    sendMessage(_clientSocket, name.c_str());

    // in that reply, server will ask us to write his name
    std::cout << getReply(_clientSocket);
    std::cin >> bet;
    sendMessage(_clientSocket, bet.c_str());

    while(true);
}


std::string Client::getReply(int fd) {
    char buffer[BUFSIZE];
    int nbytesrecv = recv(fd, buffer, BUFSIZE, 0);
    buffer[nbytesrecv] = '\0';

    if (nbytesrecv <= 0) {
        //problem
        if (0 == nbytesrecv) {
            close(fd); //well then, bye bye.
            return "error";
        } 
        else {
            perror("client recv() failed");
        }
        close(fd); //close connection to client
        return "error";
    }
    return buffer;
}

int Client::sendMessage(int fd, const char *messageBuffer) {
    return send(fd, messageBuffer,strlen(messageBuffer),0);
}