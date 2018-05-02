/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TmpServer.h
 * Author: Jakub Pekar
 *
 * Created on Utorok, 2017, decembra 19, 12:50
 */

#ifndef TMPSERVER_H
#define TMPSERVER_H

#include <string>

#include "Socket.h"
#include "Mutex.h"
#include "TmpSenzor.h"
#include "Modul.h"
#include "../lbr_c++/definitions.h"
#include "Condition.h"
#include <vector>
#include "MySQL.h"


class TmpServer : public Thread{
private:
    Socket *serverSocket;
    Socket *serverSocketSettings;
    Mutex *aMutex;
    Condition *conIsDisconnect;
    MySQL *connectorToTmpDb;
    MySQL *connectorToStateDb;
    vector <TmpSenzor*> sensors;
    bool *serverWork;
    bool work;
    
public:
    TmpServer(bool *paServerWork, MySQL *paConnector);
    void setPeriodOnSensor(std::string name, uint32_t period);
    TmpServer(const TmpServer& orig) = delete;
private:
    
    void operator=(const TmpServer &right);
    int getCountOfSensors(){return sensors.size();}
    
    
    virtual void threadMain();
    virtual void threadControl();
public:
    virtual ~TmpServer();
};

#endif /* TMPSERVER_H */

