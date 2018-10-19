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
#include "Definitions.h"
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
    /**
    * Function set the state of light on module
    * @param pLightOnstring - RGB code for LED diodes
    * @param light - state of main light. True - turned on
    */
    void setStateOfLight(bool light,string pLightOnstring = " ");
    /**
    * Function send alert to module
    */
    bool sendAlert();
    /**
    * Function activated alarm on the module
    * @param start - value of security state, true - alarm is active
    * @return true if set is successful, false otherwise
    */
    bool setAlarm(bool start);
    /**
    * Function return name of module
    * @return const string name of module
    */
    string getName() const {return name;}
    virtual ~Light();
private:
    
    virtual void threadMain();
    Light(const Light& orig) = delete;
};

#endif /* LIGHT_H */

