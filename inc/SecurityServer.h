/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SecurityServer.h
 * Author: Jakub Pekar
 *
 * Created on March 19, 2018, 7:20 PM
 */

#ifndef SECURITYSERVER_H
#define SECURITYSERVER_H

#include <vector>

#include "Thread.h"
#include "Socket.h"
#include "MySQL.h"
#include "MVMSensor.h"
#include "Condition.h"
#include "LightServer.h"


class SecurityServer : public Thread {
private:
    Socket *m_serverSocket;
    Mutex *m_mutex;
    Condition *m_conIsDisconnect;
    MySQL *m_connectorToStateDb;
    std::vector<MVMSensor*> sensors;
    Condition *m_alarm;
    bool *serverWork;
    bool *alarmActive;
    LightServer *m_lightServer;
    uint16_t* m_timeForLight;
public:
    SecurityServer(bool *pServerWork,Condition *alarm, bool *pAlarmActive,LightServer *pLightServer, uint16_t *pTimeForLight, MySQL *pConnector);
    virtual ~SecurityServer();
    /**
    * Function asigns the light to mvm module
    * @param sensorName - name of mvm module
    * @param lightName - name of light module
    * @return true if add is successful, false otherwise
    */
    bool addConection(std::string sensorName, std::string lightName);
    /**
    * Function set the state of lights on the module
    * @param lightName - name of light  module
    * @return true if remove is successful, false otherwise
    */
    bool removeConnection(std::string lightName);
    
    SecurityServer(const SecurityServer& orig) = delete;
private:
    virtual void threadMain();
    virtual void threadControl();
};

#endif /* SECURITYSERVER_H */

