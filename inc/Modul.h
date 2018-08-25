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

class Modul {
public:
    Modul(){};
    /**
    * Function returns value of atribute clientDocsonnect
    * @return bool of connection state
    */
    bool isClientDisconnected() const {
        return clientDisconnected;
    }
    /**
    * Function set the name of object
    * @param name - setted name
    */
    void SetName(string name) {
        this->name = name;
    }

    ~Modul(){};
protected:
    Socket *modulSock;
    string name;
    bool clientDisconnected;
};

#endif /* MODUL_H */

