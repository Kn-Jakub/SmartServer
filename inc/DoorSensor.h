/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DoorSensor.h
 * Author: Jakub Pekar
 *
 */

#ifndef DOORSENSOR_H
#define DOORSENSOR_H

#include "MySQL.h"
#include "Condition.h"
#include "Socket.h"
#include "Thread.h"
#include "Modul.h"
#include "LightServer.h"

class DoorSensor : public Thread, Modul {
public:
    DoorSensor(Socket* socketDescriptor,
            std::string pName,
            bool *pAlarmActive,
            Condition *pAlarm,
            Condition *paCondition,
            MySQL *paConnectToState);
    virtual void threadMain();
    DoorSensor(const DoorSensor& orig) = delete;
    virtual ~DoorSensor();

    /**
     * Function for get name of modul
     * @return string of name
     */
    std::string getName() const {
        return name;
    }

private:
    MySQL *m_connectorToStateDB;
    Condition *conIsDisconnect;
    Condition *m_alarm;
    bool *alarmActive;
};

#endif /* DOORSENSOR_H */

