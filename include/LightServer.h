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

#include "Socket.h"
#include "Mutex.h"
#include "Light.h"
#include "Definitions.h"
#include "MySQL.h"
#include <vector>

class LightServer : public Thread{
public:
    LightServer(bool *paServerWork, MySQL *paConnector);
    virtual ~LightServer();
    /**
    * Function set the state of lights on the module
    * @param state - RGB code for LED diodes
    * @param lightOn - state of main light. True - turned on
    * @param paNameSensor - name for identification of module
    * @return true if set is successful, false otherwise
    */
    bool setLight(string state,bool lightOn, string paNameSensor);
    /**
    * Function send alert to light modules
    * @return true if alert is successful sended, false otherwise
    */
    bool sendAlert();
    /**
    * Function actived alarm on all conected modules
    * @param start - is state of alarm, true - alarm is actived
    * @return true if alarm message is successful sended, false otherwise
    */
    bool alarm(bool start);
    Light *getLight(std::string pName);
    
private:
    void operator=(const LightServer &right);
    virtual void threadMain();
    virtual void threadControl();
    LightServer(const LightServer& orig) = delete;

private:
    Socket *serverSocket;
    MySQL *connectorToState;
    Mutex *aMutex;
    Condition *conIsDisconnect;
    vector <Light*> moduls;
    bool *serverWork;
    bool work;
};

#endif /* LIGHTSERVER_H */

