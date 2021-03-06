/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DoorServer.h
 * Author: jakub
 *
 */

#ifndef DOORSERVER_H
#define DOORSERVER_H

#include "Thread.h"
#include "Socket.h"
#include "MySQL.h"
#include "DoorSensor.h"
#include "Condition.h"
#include "LightServer.h"
#include <vector>

class DoorServer : public Thread {

public:
    DoorServer(bool *pServerWork, bool *pAlarmActive, Condition *alarm, MySQL *pConnector);
    DoorServer(const DoorServer& orig) = delete;
    virtual ~DoorServer();
private:
    virtual void threadMain();
    virtual void threadControl();
private:
    Socket *m_serverSocket;
    Mutex *m_mutex;
    Condition *m_conIsDisconnect;
    MySQL *m_connectorToStateDb;
    Condition *m_alarm;
    std::vector<DoorSensor*> sensors;
    bool *serverWork;
    bool *alarmActive;
};

#endif /* DOORSERVER_H */

