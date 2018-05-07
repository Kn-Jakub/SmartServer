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

    
public:
    Socket(int port);
    Socket(int port, int socketDescriptor);
    Socket(int port, char* address,bool *connectSucces);
  
    Socket(const Socket& orig) = delete;
    void operator=(const Socket &right) = delete;   
    /**
    * Function return the socket value
    * @return socket descriptor
    */
    int getSocket() const {return sock;}
      /**
    * Function set the socket value
    * @param sock - new value of socket descriptor
    */
    void setSock(int sock) { this->sock = sock;}
      /**
    * Function set server socket to listening state
    */
    void serverListen();
      /**
    * Function accepted the client request
    * @return socket descriptor of client
    */
    int connectClient();
      /**
    * Function sends data from server to client
    * @param buffer - pointer on the sending data
    * @param size - count of sending bytes
    */
    void send(void *buffer, uint16_t size);
    /**
    * Function receives data from client
    * @param buffer - pointer on the receiving data
    * @param size - size of max receiving bytes
    * @return count of receiving bytes
    */
    int recieve(void *buffer, uint16_t size);
    /**
    * Function function set the socket to the inactiv mode. Socket can`t send or receive data.
    */
    void shutdownSock();
    ~Socket();
private:
    int port;
    int sock;
    

};

#endif /* SERVERSOCKET_H */

