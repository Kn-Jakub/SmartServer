/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ServerSocket.h
 * Author: Jakub Pekar
 *
 * Created on Utorok, 2017, decembra 19, 10:22
 */

#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H
#include "../lbr_c++/definitions.h"
#include <string>
#include <cstdint>


using namespace std;

class Socket {
private:
    int port;
    int sock;
    
public:
    Socket(int port);
    Socket(int port, int socketDescriptor);
    Socket(int port, char* address,bool *connectSucces);
    
private:
    Socket(const Socket& orig);
    void operator=(const Socket &right);
    
    
public:
    //int getPort() const {return port;}
    int getSocket() const {return sock;}
    void setSock(int sock) {
        this->sock = sock;
    }
    bool setSocketBlockingEnabled(int fd, bool blocking);
   
    void serverListen();
    int connectClient();
    void send(void *buffer, uint16_t size);
    int recieve(void *buffer, uint16_t size);
    
    void shutdownSock();
    ~Socket();
private:
    

};

#endif /* SERVERSOCKET_H */

