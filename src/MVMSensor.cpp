/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MVMSensor.cpp
 * Author: jakub
 * 
 * Created on March 19, 2018, 7:21 PM
 */

#include <string.h>
#include <unistd.h>

#include "MVMSensor.h"
#include "Modul.h"
#include "Thread.h"
#include "Logger.h"

MVMSensor::MVMSensor(Socket* socketDescriptor, std::string pName, bool* pAlarmActive, uint16_t* pTimeForLight, LightServer *pLightServer, Condition *pAlarm, Condition *paCondition, MySQL *paConnectToState)
: m_connectorToStateDB(paConnectToState),
conIsDisconnect(paCondition),
controledLights(),
m_lightServer(pLightServer),
alarm(pAlarm),
timeForLight(pTimeForLight),
alarmActive(pAlarmActive)
{
    modulSock = socketDescriptor;
    name = pName;
    clientDisconnected = false;
    StartInternalThread();
}

void MVMSensor::threadMain() {
    uint8_t buffer[BUFF_BIG_LENGTH];
    string sendingBF;
    LOG_INFO("MVMS::(", name, ")obdrzal som meno: ", name);
    sendingBF = std::string(1, ANS_ACK);
    sendingBF += name;
    modulSock->send((uint8_t*) sendingBF.c_str(), sendingBF.size());

    /*Connecting of MVMSensor to the state database*/
    if (m_connectorToStateDB->insertTo(string(DBTAB_MVM_MODULES), name, true)) {
        LOG_DEBUG("MVMS:: (", name, ") Nastavujem stav v state databaze");
    } else {
        LOG_DEBUG("MVMS:: (", name, ") Vytvorena novy riadok v tabulke");
    }

    while (!clientDisconnected) {
        if (modulSock->recieve(buffer, BUFF_LENGTH) > 0) {
            LOG_TRACE("MVMS:: (", name, ") Prijata sprava ",to_string(*buffer));
            //iteratorMap = controledLights.begin();
            if (*alarmActive) {
                if (buffer[0] == 130) {
                    LOG_WARN("MVMS:: (", name, ") Alarm Alert");
                    alarm->signal();
                    buffer[0] = 130;
                    buffer[1] = 0;
                    buffer[2] = 0;
                    modulSock->send(buffer, 3);
                    LOG_TRACE("MVMS:: (", name, ") Odosiela cas o neaktivite");
                }

            } else {
                if (buffer[0] == 130) {
                    for (std::string act : controledLights) {
                        m_lightServer->setLight(" ", true, act);
                    }
                    memcpy(buffer + 1, timeForLight, sizeof (uint16_t));
                    modulSock->send(buffer, 3);
                    LOG_INFO("MVMS:: (", name, ")Senzor aktivny, zapina svetlo (", to_string(*timeForLight), ")");

                } else if (buffer[0] == 131) {
                    for (int i = (controledLights.size() - 1); i >= 0; i--) {
                        if (!m_lightServer->setLight(" ", false, controledLights.at(i))) {
                            controledLights.erase(controledLights.begin() + i);
                        }
                    }
                }
            }
        } else {
            clientDisconnected = true;
        }

    }
    LOG_INFO("MVMS:: (", name, ") Modul odpojeny");
    conIsDisconnect->signal();
}

void MVMSensor::addLight(std::string light) {
    controledLights.push_back(light);

}

bool MVMSensor::removeLight(std::string& pName) {
    for (std::vector<std::string>::iterator it =controledLights.begin();it <controledLights.end(); it++) {
        if(*it == pName){
            controledLights.erase(it);
        }
    }
    return true;
}

MVMSensor::~MVMSensor() {
    delete modulSock;
    if (m_connectorToStateDB->insertTo(string(DBTAB_MVM_MODULES), name, false)) {
        LOG_DEBUG("MVMS:: (", name, ") Set the State in the state Database(false) \n");

    } else {
        LOG_DEBUG("MVMS:: (", name, ") Some Error with set of exit stete to DB");
    }
    WaitForInternalThreadToExit();
}

