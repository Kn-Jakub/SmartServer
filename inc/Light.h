/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Light.h
 * Author: Jakub Pekar
 *
 */

#ifndef LIGHT_H
#define LIGHT_H
#include "../lbr_c++/definitions.h"
#include "Thread.h"
#include "Modul.h"
#include "Condition.h"
#include "MySQL.h"

class Light : public Thread , Modul {
private:
    bool lightOn;
    string lightColor;
    Condition *conIsDisconnect;
    MySQL *connectorToState;
    
public:
    Light(Socket *socketDescriptor, string recvName, Condition* paConIsDisconnect, MySQL *paConnectorState);
    void setStateOfLight(bool pLightOnstring,string light = " ");
    bool sendAlert();
    bool setAlarm(bool start);
    string getName() const {return name;}
    virtual ~Light();
private:
    
    virtual void threadMain();
    Light(const Light& orig) = delete;
};

#endif /* LIGHT_H */

