/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SecurityManager.cpp
 * Author: jakub
 * 
 * Created on April 9, 2018, 9:51 PM
 */

#include <unistd.h>

#include "SecurityManager.h"

SecurityManager::SecurityManager(bool *pServerWork,bool *pAlarmActive,LightServer *pLServer,MySQL *pConnector, Condition *runAlarm)
:m_serverWork(pServerWork),
m_alarmActive(pAlarmActive),
m_ligthServer(pLServer),
m_runAlarm(runAlarm),
m_alarmMutex(new Mutex()),
counterOfReactionAlarm(30),
m_connetorToStateDB(pConnector)
{
    StartInternalThread();
}

void SecurityManager::threadMain() {
    m_alarmMutex->lock();
    bool isActive = false;
    while(*m_serverWork){
        m_runAlarm->wait(m_alarmMutex);
        if(*m_alarmActive){
            if(!isActive){
            m_ligthServer->sendAlert();
            for(int i =0;i < counterOfReactionAlarm;i++){
                if(!*m_alarmActive)break;
                sleep(1);
            }
            }
            if(*m_alarmActive){
                m_ligthServer->alarm(true);
                isActive = true;
            } else {
                m_ligthServer->alarm(false);
                isActive = false;
            }
        }else {
            if(isActive){
                m_ligthServer->alarm(false);
            }
            isActive = false;
        }
    }
    m_alarmMutex->unlock();
}

bool SecurityManager::getAlarmState() const {
    return *m_alarmActive;
}

uint16_t SecurityManager::getReactionTime() const {
    return counterOfReactionAlarm;
}

void SecurityManager::setAlarmState(bool state) {
    *m_alarmActive = state;
}

void SecurityManager::setReactionTime(uint16_t time) {
    counterOfReactionAlarm = time;
}

SecurityManager::~SecurityManager() {
    WaitForInternalThreadToExit();
    delete m_runAlarm;
    delete m_alarmMutex;
}

