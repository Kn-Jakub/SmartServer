/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TmpServer.cpp
 * Author: Jakub Pekar
 * 
 * Created on Utorok, 2017, decembra 19, 12:50
 */

#include "TmpServer.h"
#include "../lbr_c++/definitions.h"
#include "Logger.h"

TmpServer::TmpServer(bool *paServerWork, MySQL *paConnector)
:serverSocket(new Socket(PORT_TMP)),
serverSocketSettings(new Socket(PORT_TMP_2)),
aMutex(new Mutex()),
conIsDisconnect(new Condition()),
connectorToTmpDb(new MySQL()),
connectorToStateDb(paConnector),
sensors(),
serverWork(paServerWork),
work(true)
{
    connectorToTmpDb->initMysql(DB_SERVER_TEMPERATURE);
    StartInternalThread();
    StartControlThread();
    LOG_DEBUG("TMPServer::TMP server vytvoreny");
}

void TmpServer::threadMain() {

    serverSocket->serverListen();
    serverSocketSettings->serverListen();
    int clientDescriptor;
    Socket* varClient;
    char buffer[1024];
    bool correctName = true;
    uint16_t counter;
    string recvName;
    std::string newName;
    size_t pos;

    while (*serverWork) {
        clientDescriptor = serverSocket->connectClient();
        
        int setingClient = serverSocketSettings->connectClient();
        varClient = new Socket(PORT_TMP, clientDescriptor);
        counter =1;

        if (*serverWork) {
            if (varClient->recieve(buffer, 1024)) {
                recvName = string(buffer+1);
                LOG_TRACE("TMPServer:: Obdrzal som meno ", recvName);
                while((pos = recvName.find(' ')) != std::string::npos){
                    recvName.replace(pos,1,"_");
                }
                newName = recvName;
            }
             do{
                for (TmpSenzor* act : sensors) {
                    if (act->getName() == newName) {
                        LOG_INFO("TMPServer:: Obdrzal som neplatne meno generujem nove");
                        if(pos = recvName.find('-') != std::string::npos)
                            newName = recvName.substr(0,pos);
                        newName += "-";
                        newName += to_string(counter);
                        counter++;
                        correctName = false;
                        break;
                    }else {
                        correctName = true;
                    }
                }
                
            }while (!correctName);
                
            aMutex->lock();
            sensors.push_back(new TmpSenzor(varClient, newName, conIsDisconnect, connectorToTmpDb, connectorToStateDb, setingClient));
            aMutex->unlock();

            LOG_INFO("TMPServer::Pripojil sa novy senzor");
        }
    }
    LOG_INFO("TMPServer::Ukoncujem threadMain");
    conIsDisconnect->signal();
}

void TmpServer::setPeriodOnSensor(std::string name, uint32_t period) {
    bool setSuccess = false;
    for (unsigned int i = 0; i < sensors.size(); i++) {
       // if (sensors.at(i)->getName() == name) {
            LOG_INFO("TMPServer:: nastavujem periodu %s, -> %d", sensors.at(i)->getName(), period);
            sensors.at(i)->setPeriod(period);
            setSuccess = true;
           // break;
      //  }
    }
    if (!setSuccess){
        if(sensors.size() == 0){
            LOG_INFO("TMPServer:: Server nema pripojeny ziaden senzor teploty");
        }else{
            LOG_INFO("TMPServer:: senzor neexituje");

        }
    }
}
//Control the state of conect sensors. If is sensor disconect, this thread erese the pointer on the sensors from vector

void TmpServer::threadControl() {
    aMutex->lock();
    while (*serverWork) {
        conIsDisconnect->wait(aMutex);
        for (int i = (sensors.size() - 1); i >= 0; i--) {
            if (!*serverWork) {
                delete sensors.at(i);
                sensors.erase(sensors.begin() + i);
            } else if (((Modul*) sensors.at(i))->isClientDisconnected()) {
                delete sensors.at(i);
                sensors.erase(sensors.begin() + i);
            }
        }
    }
    LOG_INFO("TMPServer::Ukoncujem threadControl");
    aMutex->unlock();
}

TmpServer::~TmpServer() {

    WaitForInternalThreadToExit();
    WaitForControlThreadToExit();
    delete conIsDisconnect;
    delete aMutex;
    delete serverSocket;
    delete connectorToTmpDb;
    LOG_DEBUG("TMPServer::Destruktor TMPServer vykonany");
}

