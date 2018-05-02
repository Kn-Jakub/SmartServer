/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Modul.h
 * Author: Jakub Pekar
 *
 * Created on Utorok, 2018, januára 2, 15:28
 */

#ifndef MODUL_H
#define MODUL_H

#include <string>
#include "Socket.h"
#include "../lbr_c++/definitions.h"
class Modul
{
protected:
    Socket *modulSock;
    string name;
    //string ipAddr;
    bool clientDisconnected;
public:
    Modul() {}
    /*
    void SetIpAddr(string ipAddr) {
        this->ipAddr = ipAddr;
    }

    void SetModulSock(Socket modulSock) {
        this->modulSock = modulSock;
    }
*/  bool isClientDisconnected() const { return clientDisconnected;}
    void SetName(string name) {
        this->name = name;
    }
    


    ~Modul(){ }

};

#endif /* MODUL_H */

