/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ServerSocket.cpp
 * Author: Jakub Pekar
 * 
 * Created on Utorok, 2017, decembra 19, 10:22
 */



#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/unistd.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
//#include "ServerExceptions.h"
#include "Socket.h"
#include "Logger.h"

// Craeting of server socket

Socket::Socket(int paPort) : port(paPort) {
    try {
        if (port <= 0) {
            throw EX_PORT_WRONG_NUMBER;
        }

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            throw EX_SOCK_CREATING;
        }

        struct sockaddr_in serverAdresa;
        serverAdresa.sin_family = AF_INET;
        serverAdresa.sin_addr.s_addr = INADDR_ANY;
        serverAdresa.sin_port = htons(port);
        
        int option = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        
        if (bind(sock, (struct sockaddr*) &serverAdresa, sizeof (serverAdresa)) < 0) {
            throw EX_BIND;
        }
    } catch (int e) {
        if (e == EX_PORT_WRONG_NUMBER) LOG_ERROR("[SOCKET]Wrong port number");
        if (e == EX_SOCK_CREATING) LOG_ERROR("[SOCKET]Problem with creating of socket");
        if (e == EX_BIND) LOG_ERROR("[SOCKET]Bind");
        std::cout << " " << std::strerror(errno) << endl;
    }
}

//Creating of Klient socket

Socket::Socket(int paPort, char* address, bool* connectSuccess) : port(paPort) {
    if (!address) {

    }
    struct hostent* server = gethostbyname(address);
    if (server == NULL) {
        LOG_ERROR("[Socket]CONNECTING::Can`t connect to server");
        *connectSuccess = false;
    } //printError("Server does not exist.");

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        LOG_ERROR("[Socket]create::Can`t create socket");
        *connectSuccess = false;
    }//printError("ERROR - socket.");

    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof (server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    bcopy((char*) server->h_addr_list[0], &(server_addr.sin_addr.s_addr), server->h_length);

    if (connect(sock, (struct sockaddr*) &server_addr, sizeof (server_addr)) < 0) {
        LOG_ALL("[Socket]ERROR - connect.");
        *connectSuccess = false;
    } else {
        *connectSuccess = true;
        LOG_INFO("[Socket]Connecting to server - successfully.");
    }
}

Socket::Socket(int port, int socketDescriptor) : port(port), sock(socketDescriptor) {
}


void Socket::serverListen() {
    listen(sock, 10);
}
// return client socket descriptor

int Socket::connectClient() {
    int socketClient;

    try {
        struct sockaddr_in clientAdresa;
        socklen_t dlzkaClientAdresy = sizeof (clientAdresa);

        LOG_ALL("SOCKET::[", sock, "] Som pred accept");
        if ((socketClient = accept(this->sock, (struct sockaddr*) &clientAdresa, &dlzkaClientAdresy)) < 0) {
            throw EX_SOCK_ACCEPT;
        }
        LOG_TRACE("SOCKET::[", sock, "] Client was accepted: ", inet_ntoa(clientAdresa.sin_addr), ", ", ntohs(clientAdresa.sin_port));

    } catch (int e) {
        if (e == EX_SOCK_ACCEPT) LOG_ERROR("ERROR::Problem with accepting clientSocket");
    }
    return socketClient;
}

void Socket::send(void *buffer, uint16_t size) {
    LOG_TRACE("SOCKET_SEND::(", sock, ")");
    write(sock, buffer, size);
}

int Socket::recieve(void *buffer, uint16_t size) {
    int valread = read(sock, buffer, size);
    return valread;
}
void Socket::shutdownSock() {
    shutdown(sock,2);
}

Socket::~Socket() {
    close(sock);
}

