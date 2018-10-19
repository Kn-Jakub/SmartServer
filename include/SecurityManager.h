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
    
public:
    SecurityManager(bool *pServerWor,bool *alarmActive,LightServer *pLServer,MySQL *pConnector, Condition *runAlarm);
    SecurityManager(const SecurityManager& orig) = delete;
    virtual ~SecurityManager();
    /**
    * Function set the value of reaction time for alarm. It is time between alert sending and alarm sending.
    * @param time - it is setting value
    */
    void setReactionTime(uint16_t time);
    /**
    * Function get the attribute value
    * @return reaction time between alert sending and alarm sendig
    */
    uint16_t getReactionTime()const;
     /**
    * Function get the attribute value alarm state
    * @return bool value od alarmState, true is active
    */
    bool getAlarmState()const;
     /**
    * Function set the attribute value alarmState
    * @param pState - setting value
    */
    void setAlarmState(bool pState);
private:
    virtual void threadMain();
private:
        bool *m_serverWork;
        bool *m_alarmActive;
        LightServer * m_ligthServer;
        Condition *m_runAlarm;
        Mutex *m_alarmMutex;
        uint16_t counterOfReactionAlarm;
        MySQL *m_connetorToStateDB;    
};

#endif /* SECURITYMANAGER_H */

