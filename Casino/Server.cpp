#include "Server.hpp"

Server::Server()
{
    setup(DEFAULT_PORT);
}

Server::Server(int port)
{
    setup(port);
}

Server::~Server()
{
	close(mastersocket_fd);
}


std::string Server::getReply(int fd) {
    char buffer[INPUT_BUFFER_SIZE];
    int nbytesrecv = recv(fd, buffer, INPUT_BUFFER_SIZE, 0);
    buffer[nbytesrecv] = '\0';

    if (nbytesrecv == 0) {
        closeConnection(fd);
        throw Exception("o bytes received");
    } 
    else if (nbytesrecv < 0) {
        closeConnection(fd);
        throw Exception("<0 bytes recevied");
    }

    return std::string(buffer);
}


// setup of server doing some stuff: 
void Server::setup(int port)
{
    mastersocket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (mastersocket_fd < 0) {
        perror("Socket creation failed");
    }

    FD_ZERO(&masterfds);
    FD_ZERO(&tempfds);

    memset(&servaddr, 0, sizeof (servaddr)); //bzero
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(port);

    bzero(input_buffer,INPUT_BUFFER_SIZE); //zero the input buffer before use to avoid random data appearing in first receives
}

void Server::initializeSocket()
{
	int opt_value = 1;
	int ret_test = setsockopt(mastersocket_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt_value, sizeof (int));

	if (ret_test < 0) {
        perror("[SERVER] [ERROR] setsockopt() failed");
		shutdown();
	}
}

void Server::bindSocket()
{
	int bind_ret = bind(mastersocket_fd, (struct sockaddr*) &servaddr, sizeof (servaddr));

	if (bind_ret < 0) {
		perror("[SERVER] [ERROR] bind() failed");
	}
	FD_SET(mastersocket_fd, &masterfds); //insert the master socket file-descriptor into the master fd-set
	maxfd = mastersocket_fd; //set the current known maximum file descriptor count
}

void Server::startListen()
{
	int listen_ret = listen(mastersocket_fd, 3);

	if (listen_ret < 0) {
		perror("[SERVER] [ERROR] listen() failed");
	}
}

void Server::shutdown()
{
	int close_ret = close(mastersocket_fd);
}


int Server::handleNewConnection()
{
	socklen_t addrlen = sizeof (client_addr);
	tempsocket_fd = accept(mastersocket_fd, (struct sockaddr*) &client_addr, &addrlen);
    	
	if (tempsocket_fd < 0) {
        	perror("[SERVER] [ERROR] accept() failed");
	} 
	else {
        // setting new connection descripton to set
        FD_SET(tempsocket_fd, &masterfds);
		//increment the maximum known file descriptor (select() needs it)
        if (tempsocket_fd > maxfd) {
            maxfd = tempsocket_fd;
        }
    }

    return tempsocket_fd;
}

void Server::closeConnection(int fd) {
    close(fd); //close connection to client
	FD_CLR(fd, &masterfds); //clear the client fd from fd set
}


void Server::init()
{
    initializeSocket();
    bindSocket();
    startListen();
}

uint16_t Server::sendMessage(int fd, char *messageBuffer) {
    // const std::lock_guard<std::mutex> lock(sendMsgMutex);
    return send(fd, messageBuffer,strlen(messageBuffer),0);
}

uint16_t Server::sendMessage(int fd, const char *messageBuffer) {
    // const std::lock_guard<std::mutex> lock(sendMsgMutex);
    return send(fd, messageBuffer,strlen(messageBuffer),0);
}