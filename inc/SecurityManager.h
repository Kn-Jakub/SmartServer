/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SecurityManager.h
 * Author: jakub
 *
 * Created on April 9, 2018, 9:51 PM
 */

#ifndef SECURITYMANAGER_H
#define SECURITYMANAGER_H

#include "Thread.h"
#include "LightServer.h"


class SecurityManager: public Thread {
    private:
        bool *m_serverWork;
        bool *m_alarmActive;
        LightServer * m_ligthServer;
        Condition *m_runAlarm;
        Mutex *m_alarmMutex;
        uint16_t counterOfReactionAlarm;
        MySQL *m_connetorToStateDB;
public:
    SecurityManager(bool *pServerWor,bool *alarmActive,LightServer *pLServer,MySQL *pConnector, Condition *runAlarm);
    SecurityManager(const SecurityManager& orig) = delete;
    virtual ~SecurityManager();
    
    void setReactionTime(uint16_t time);
    uint16_t getReactionTime()const;
    bool getAlarmState()const;
    void setAlarmState(bool pState);
private:
    virtual void threadMain();
};

#endif /* SECURITYMANAGER_H */

