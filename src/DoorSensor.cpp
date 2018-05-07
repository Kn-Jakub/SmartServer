/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DoorSensor.cpp
 * Author: Jakub Pekar
 * 
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
conIsDisconnect(paCondition),
m_alarm(pAlarm),
alarmActive(pAlarmActive){
    modulSock = socketDescriptor;
    name = pName;
    clientDisconnected = false;

    StartInternalThread();
}
/*
 * Funkcia hlavneho vlakna v ktorej objekt prijima signal pri zmene na senzore
 */
void DoorSensor::threadMain() {
    uint8_t buffer[BUFF_BIG_LENGTH];
    string sendingBF;
    LOG_INFO("Door::(", name, ")obdrzal som meno: ", name);

    sendingBF = std::string(1,ANS_ACK);
    sendingBF += name;
    modulSock->send((uint8_t*) sendingBF.c_str(), sendingBF.size());
    
    /*Pripojenie DoorSensora a zapis do databazy*/
    if (m_connectorToStateDB->insertTo(string(DBTAB_MVM_MODULES), name, true)) {
        LOG_DEBUG("Door:: (", name, ") Nastavujem stav v state databaze");
    } else {
        LOG_DEBUG("Door:: (", name, ") Vytvaram novy riadok v state databaze");
    }

    while (!clientDisconnected) {
        if (modulSock->recieve(buffer, BUFF_LENGTH) > 0) {
            LOG_DEBUG("Door:: (", name, ") prijata sprava: ",to_string(*buffer));
            if (*alarmActive) {
                LOG_WARN("Door:: (", name, ") Alarm Active");
                m_alarm->signal(); 
            } 
        } else {
            clientDisconnected = true;
        }

    }
    LOG_INFO("Door:: (", name, ") Modul odpojeny");
    conIsDisconnect->signal();
}

DoorSensor::~DoorSensor() {
}

