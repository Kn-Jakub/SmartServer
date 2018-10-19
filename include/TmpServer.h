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
#include "Definitions.h"
#include "Condition.h"
#include <vector>
#include "MySQL.h"


class TmpServer : public Thread{

    
public:
    TmpServer(bool *paServerWork, MySQL *paConnector);
    virtual ~TmpServer();
    /**
    * Function set the periode on the sensor from the vector
    * @param name - name of tmp module
    * @param period - value of setting period
    */
    void setPeriodOnSensor(std::string name, uint32_t period);
    TmpServer(const TmpServer& orig) = delete;
private:
    void operator=(const TmpServer &right);
    virtual void threadMain();
    virtual void threadControl();
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
    
};

#endif /* TMPSERVER_H */

