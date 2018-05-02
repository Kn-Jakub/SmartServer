/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LightServer.h
 * Author: Jakub Pekar
 *
 * Created on Streda, 2018, januára 3, 13:51
 */

#ifndef LIGHTSERVER_H
#define LIGHTSERVER_H

//class SecurityServer;

#include "Socket.h"
#include "Mutex.h"
#include "Light.h"
#include "../lbr_c++/definitions.h"
#include "MySQL.h"
#include <vector>

class LightServer : public Thread{
private:
    Socket *serverSocket;
    MySQL *connectorToState;
    Mutex *aMutex;
    Condition *conIsDisconnect;
    vector <Light*> moduls;
    bool *serverWork;
    bool work;
public:
    LightServer(bool *paServerWork, MySQL *paConnector);
    bool setLight(string state,bool lightOn, string paNameSensor);
    bool sendAlert();
    bool alarm(bool start);
    Light *getLight(std::string pName);
    
private:
    void operator=(const LightServer &right);
    virtual void threadMain();
    virtual void threadControl();
    LightServer(const LightServer& orig) = delete;
public:
    virtual ~LightServer();
};

#endif /* LIGHTSERVER_H */

