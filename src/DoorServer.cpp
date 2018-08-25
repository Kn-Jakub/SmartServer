/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DoorServer.cpp
 * Author: Jakub Pekar
 * 
 */

#include "DoorServer.h"
#include "Logger.h"

DoorServer::DoorServer(bool *pServerWork, bool *pAlarmActive, Condition *pAlarm, MySQL *pConnector) 
:m_serverSocket(new Socket(PORT_DOOR_SENZOR)),

m_mutex(new Mutex()),
m_conIsDisconnect(new Condition()),
m_connectorToStateDb(pConnector),
        m_alarm(pAlarm),
        sensors(),
        serverWork(pServerWork),
        alarmActive(pAlarmActive)
{
    StartInternalThread();
    StartControlThread();
    LOG_DEBUG("DoorServer: server vytvoreny");
}
/*
 * Metoda hlavneho vlakna pre obsluhu klientov
 */
void DoorServer::threadMain() {
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
        clientSocket = new Socket(PORT_DOOR_SENZOR, clientDescriptor);
        clientSocket->recieve(buffer, BUFF_LENGTH);
        if (*serverWork) {
            counter =1;
            recvName = std::string(buffer + 1);
            LOG_TRACE("DoorServer: Obdrzal som meno ", recvName);
            newName = recvName;
            do {
                for (DoorSensor *act : sensors) {
                    if (act->getName() == newName) {
                        LOG_WARN("DoorServer:: meno je uz pouzite generujem nove");
                        if((pos = recvName.find('-')) != std::string::npos)
                            recvName = recvName.substr(0,pos);
                        newName = recvName;
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
            sensors.push_back(new DoorSensor(clientSocket, newName, alarmActive,m_alarm, m_conIsDisconnect, m_connectorToStateDb));
            m_mutex->unlock();

            LOG_INFO("DoorServer:: Pripojil sa novy senzor");
        } else {
            delete clientSocket;
        }
    }
    LOG_INFO("DoorServer:: Ukoncujem threadMain");
    m_conIsDisconnect->signal();
}
/*
 * Metoda kontrolneho vlakna, ktore odstranuje neaktivne moduly z vektora
 */
void DoorServer::threadControl() {
     m_mutex->lock();
    while (*serverWork) {
        //while (sensors.size() != 0) {
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
        //}
    }
    LOG_INFO("DoorServer:: Ukoncujem threadControl");
    m_mutex->unlock();
}



DoorServer::~DoorServer() {
    WaitForInternalThreadToExit();
    delete m_conIsDisconnect;
    delete m_serverSocket;
    LOG_DEBUG("DoorServer:: Destruktor Door Servera uspesne vykonany");
}

