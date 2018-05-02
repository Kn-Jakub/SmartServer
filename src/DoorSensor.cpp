/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DoorSensor.cpp
 * Author: jakub
 * 
 * Created on April 3, 2018, 9:06 AM
 */

#include "DoorSensor.h"
#include "Logger.h"

DoorSensor::DoorSensor(Socket* socketDescriptor,
            std::string pName,
            bool *pAlarmActive,
        Condition *pAlarm,
            Condition *paCondition, 
            MySQL *paConnectToState) 
:m_connectorToStateDB(paConnectToState),
m_alarm(pAlarm){
    modulSock = socketDescriptor;
    conIsDisconnect = paCondition;
    name = pName;
    alarmActive = pAlarmActive;
    clientDisconnected = false;

    StartInternalThread();
}

void DoorSensor::threadMain() {
    uint8_t buffer[BUFF_BIG_LENGTH];
    string sendingBF;
    LOG_INFO("Door::(", name, ")obdrzal som meno: ", name);

    //buffer[0] = ANS_ACK;
    sendingBF = std::string(1,ANS_ACK);
    sendingBF += name;
    modulSock->send((uint8_t*) sendingBF.c_str(), sendingBF.size());
    
    /*Connecting of TmpSensor to the state database*/
    if (m_connectorToStateDB->insertTo(string(DBTAB_MVM_MODULES), name, true)) {
        LOG_DEBUG("Door:: (", name, ") Set the State in the state Database");
    } else {
        LOG_DEBUG("Door:: (", name, ") Create new row in the table of state");
    }

    while (!clientDisconnected) {
        if (modulSock->recieve(buffer, BUFF_LENGTH) > 0) {
            LOG_DEBUG("Door:: (", name, ") prijata sprava: ",to_string(*buffer));
            if (*alarmActive) {
                LOG_WARN("Door:: (", name, ") Alarm Active");
                m_alarm->signal();
                
            } else {
                
            }
        } else {
            clientDisconnected = true;
        }

    }
    LOG_INFO("Door:: (", name, ") Client unconnected");
    conIsDisconnect->signal();
}


DoorSensor::~DoorSensor() {
}

