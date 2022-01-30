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
	close(_mastersocket_fd);
}


std::string Server::getReply(int fd) {
    char buffer[INPUT_BUFFER_SIZE];
    int nbytesrecv = recv(fd, buffer, INPUT_BUFFER_SIZE, 0);
    buffer[nbytesrecv] = '\0';

    if (nbytesrecv == 0) {
        closeConnection(fd);
        //throw Exception("0 bytes received");
    } 
    else if (nbytesrecv < 0) {
        closeConnection(fd);
        //throw Exception("<0 bytes recevied");
    }

    return std::string(buffer);
}


// setup of server doing some stuff: 
void Server::setup(int port)
{
    _mastersocket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_mastersocket_fd < 0) {
        perror("Socket creation failed");
    }

    FD_ZERO(&_masterfds);
    FD_ZERO(&_tempfds);

    memset(&_servaddr, 0, sizeof (_servaddr)); //bzero
    _servaddr.sin_family = AF_INET;
    _servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    _servaddr.sin_port = htons(port);

    bzero(_input_buffer,INPUT_BUFFER_SIZE); //zero the input buffer before use to avoid random data appearing in first receives
}

void Server::initializeSocket()
{
	int opt_value = 1;
	int ret_test = setsockopt(_mastersocket_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &opt_value, sizeof (int));

	if (ret_test < 0) {
        perror("[SERVER] [ERROR] setsockopt() failed");
		shutdown();
	}
}

void Server::bindSocket()
{
	int bind_ret = bind(_mastersocket_fd, (struct sockaddr*) &_servaddr, sizeof (_servaddr));

	if (bind_ret < 0) {
		perror("[SERVER] [ERROR] bind() failed");
	}
	FD_SET(_mastersocket_fd, &_masterfds); //insert the master socket file-descriptor into the master fd-set
	_maxfd = _mastersocket_fd; //set the current known maximum file descriptor count
}

void Server::startListen()
{
	int listen_ret = listen(_mastersocket_fd, 3);

	if (listen_ret < 0) {
		perror("[SERVER] [ERROR] listen() failed");
	}
}

void Server::shutdown()
{
	int close_ret = close(_mastersocket_fd);
}


int Server::handleNewConnection()
{
	socklen_t addrlen = sizeof (_client_addr);
	_tempsocket_fd = accept(_mastersocket_fd, (struct sockaddr*) &_client_addr, &addrlen);
    	
	if (_tempsocket_fd < 0) {
        	perror("[SERVER] [ERROR] accept() failed");
	} 
	else {
        // setting new connection descripton to set
        FD_SET(_tempsocket_fd, &_masterfds);
		//increment the maximum known file descriptor (select() needs it)
        if (_tempsocket_fd > _maxfd) {
            _maxfd = _tempsocket_fd;
        }
    }

    return _tempsocket_fd;
}

void Server::closeConnection(int fd) {
    close(fd); //close connection to client
	FD_CLR(fd, &_masterfds); //clear the client fd from fd set
}


void Server::init()
{
    initializeSocket();
    bindSocket();
    startListen();
}

uint16_t Server::sendMessage(int fd, char *messageBuffer) const {
    // const std::lock_guard<std::mutex> lock(sendMsgMutex);
    return send(fd, messageBuffer,strlen(messageBuffer),0);
}

uint16_t Server::sendMessage(int fd, const char *messageBuffer) const {
    // const std::lock_guard<std::mutex> lock(sendMsgMutex);
    return send(fd, messageBuffer,strlen(messageBuffer),0);
}

uint16_t Server::sendMessage(int fd, std::string messageBuffer) const {
    // const std::lock_guard<std::mutex> lock(sendMsgMutex);
    return send(fd, messageBuffer.c_str(), messageBuffer.size(), 0);
}