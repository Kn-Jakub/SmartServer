/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   appComunicator.h
 * Author: Jakub Pekar
 *
 * Created on Štvrtok, 2018, januára 4, 9:25
 */

#ifndef APPCOMUNICATOR_H
#define APPCOMUNICATOR_H

#include <cstring>
#include "TmpServer.h"
#include "LightServer.h"
#include "DoorServer.h"
#include "Modul.h"
#include "MySQL.h"  
#include "Definitions.h"
#include "GlobalData.h"
#include "SecurityServer.h"
#include "SecurityManager.h"

class AppComunicator : public Thread, Modul {
public:
    AppComunicator(SERVER_DATA *data);
    virtual ~AppComunicator();

    AppComunicator(const AppComunicator& orig) = delete;
    void operator=(const AppComunicator &right) = delete;
private:
    void shutDown();

    virtual void threadMain();

    bool isUserAppDisconnect() const {
        return webServerConnect;
    }
private:
    bool* serverWork;
    LightServer *lightServer;
    TmpServer *tmpServer;
    Condition *alarmRunCondition;
    Mutex *alarmMutex;
    SecurityManager *securityManager;
    SecurityServer *secServer;
    DoorServer *doorServer;
    Socket* webServerSocket;
    bool webServerConnect;
    SERVER_DATA* serverData;

};

#endif /* APPCOMUNICATOR_H */

