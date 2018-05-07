/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SecurityServer.cpp
 * Author: Jakub Pekar
 * 
 * Created on March 19, 2018, 7:20 PM
 */

#include "SecurityServer.h"
#include "Logger.h"
//#include "LightServer.h"

SecurityServer::SecurityServer(bool *paServerWork,Condition *alarm, bool *pAlarmActive, LightServer *pLightServer, uint16_t *pTimeForLight, MySQL* paConnector)
:m_serverSocket(new Socket(PORT_MVM_SENZOR)),
        m_mutex(new Mutex()),
        m_conIsDisconnect(new Condition()),
        m_connectorToStateDb(paConnector),
        sensors(),
        m_alarm(alarm),
        serverWork(paServerWork),
        alarmActive(pAlarmActive),
        m_lightServer(pLightServer),
m_timeForLight(pTimeForLight)
{  
    StartInternalThread();
    StartControlThread();
    LOG_DEBUG("SECServer::Security server vytvoreny");
}

void SecurityServer::threadMain() {
    m_serverSocket->serverListen();
    Socket *clientSocket;
    int clientDescriptor;
    char buffer[BUFF_LENGTH];
    size_t pos;
    std::string recvName;
    std::string newName;
    int counter;
    bool correctName = true;
    while (*serverWork) {
        clientDescriptor = m_serverSocket->connectClient();
        clientSocket = new Socket(PORT_MVM_SENZOR, clientDescriptor);
        clientSocket->recieve(buffer, BUFF_LENGTH);
        if (*serverWork) {
            counter =1;
            recvName = std::string(buffer + 1);
            LOG_TRACE("SECServer:: Obdrzal som meno ", recvName);
            while((pos = recvName.find(' ')) != std::string::npos){
                    recvName.replace(pos,1,"_");
                }
            newName = recvName;
            do {
                for (MVMSensor *act : sensors) {
                    if (act->getName() == newName) {
                        LOG_WARN("SECServer:: meno je uz pouzite generujem nove");
                        if(pos = recvName.find('-') != std::string::npos)
                            newName = recvName.substr(0,pos);
                        newName += "-";
                        newName += to_string(counter);
                        counter++;
                        correctName = false;
                    } else {
                        correctName = true;
                    }
                }
            } while (!correctName);


            m_mutex->lock();
            sensors.push_back(new MVMSensor(clientSocket, newName, alarmActive, m_timeForLight,m_lightServer, m_alarm, m_conIsDisconnect, m_connectorToStateDb));
            m_mutex->unlock();

            LOG_INFO("SECServer::Pripojil sa novy senzor");
        } else {
            delete clientSocket;
        }
    }
    LOG_INFO("SECServer::Ukoncujem threadMain");
    m_conIsDisconnect->signal();
}

void SecurityServer::threadControl() {
    m_mutex->lock();
    while (*serverWork) {
        m_conIsDisconnect->wait(m_mutex);
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
    LOG_INFO("SECServer:: Ukoncujem threadControl");
    m_mutex->unlock();
}

bool SecurityServer::addConection(std::string sensorName, std::string lightName) {
    for (MVMSensor *act : sensors) {
        if (act->getName() == sensorName) {
            act->addLight(lightName);
            LOG_WARN("SECServer:: Priradujem senzor pohybu ", sensorName ," k svetlu ", lightName);
            return true;
        }
    }
    return false;
}

bool SecurityServer::removeConnection(std::string name) {
    for (MVMSensor *act : sensors) {
        if (act->removeLight(name)) {
            return true;
        }
    }
    return false;   
}


SecurityServer::~SecurityServer() {

    WaitForInternalThreadToExit();
    delete m_conIsDisconnect;
    delete m_serverSocket;
    LOG_DEBUG("SECServer::Destruktor Security Servera success");
}

