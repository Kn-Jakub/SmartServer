/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DoorSensor.h
 * Author: jakub
 *
 * Created on April 3, 2018, 9:06 AM
 */

#ifndef DOORSENSOR_H
#define DOORSENSOR_H

#include "MySQL.h"
#include "Condition.h"
#include "Socket.h"
#include "Thread.h"
#include "Modul.h"
#include "LightServer.h"
//#include "Light.h"

class DoorSensor: public Thread, Modul {
private:
    MySQL *m_connectorToStateDB;
    Condition *conIsDisconnect;
    Condition *m_alarm;
    bool *alarmActive;
public:
    DoorSensor(Socket* socketDescriptor,
            std::string pName,
            bool *pAlarmActive,
            Condition *pAlarm,
            Condition *paCondition, 
            MySQL *paConnectToState);
    DoorSensor(const DoorSensor& orig) = delete;
    virtual ~DoorSensor();
    string getName() const {    return name;    }
private:
   virtual void threadMain();
};

#endif /* DOORSENSOR_H */

