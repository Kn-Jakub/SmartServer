/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LightServer.cpp
 * Author: Jakub Pekar
 * 
 * Created on Streda, 2018, januára 3, 13:51
 */

#include <iostream>

#include "LightServer.h"
#include "Thread.h"
#include "Logger.h"

LightServer::LightServer(bool *paServerWork, MySQL *paConnector)
:serverSocket(new Socket(PORT_ACTUATOR)), 
connectorToState(paConnector),
aMutex(new Mutex()),
conIsDisconnect(new Condition()),
serverWork(paServerWork),
work(true){
    StartInternalThread();
    StartControlThread();
}
bool LightServer::sendAlert(){
    LOG_DEBUG("LIGHTServ:: **Zapinam upozornenie ALERT**");
    for (Light *act : moduls) {
        act->sendAlert();
    }
    return true;
}

bool LightServer::alarm(bool start) {
    LOG_DEBUG("LIGHTServ:: **Zapinam ALARM**");
    for (Light *act : moduls) {
        act->setAlarm(start);
    }
    return true;
}


bool LightServer::setLight(string state, bool lightOn, string paNameSensor) {
    cout << "Arraysize " << moduls.size() << " paName " << paNameSensor << endl;
    for (unsigned int i = 0; i < moduls.size(); i++) {
        cout << "ArrayName " << moduls.at(i)->getName() << " paName " << paNameSensor << endl;
        if (moduls.at(i)->getName() == paNameSensor) {
            LOG_INFO("LIGHTServ::nastavujem stav osvetlania na ", paNameSensor);
            moduls.at(i)->setStateOfLight(lightOn, state);
            return true;
        }
    }
    cout << "LightServ:: sensor dont exist" << endl;
    return false;
}

Light* LightServer::getLight(std::string pName) {
    for (Light *act : moduls) {
        if (act->getName() == pName) {
            LOG_DEBUG("LIGHTServ:: Vraciam smernik na modul ", pName);
            return act;
        }
    }
    LOG_WARN("LIGHTServ:: Modul som nenasiel ", pName);
    return NULL;
}

void LightServer::threadMain() {
    int clientDescriptor;
    Socket *socketClient;
    char buffer[BUFF_LENGTH];
    bool correctName = true;
    uint16_t counter;
    string recvName;
    std::string newName;
    std::size_t pos;
    serverSocket->serverListen();
    LOG_INFO("LIGHTserv:: som v thread main ");

    while (*serverWork) {
        clientDescriptor = serverSocket->connectClient();
        if (*serverWork) {
            counter = 1;
            socketClient = new Socket(PORT_ACTUATOR, clientDescriptor);
            if (socketClient->recieve(buffer, BUFF_LENGTH)) {
                recvName = string(buffer + 1);
                LOG_TRACE("LIGHTserv:: Obdrzal som meno ", recvName);
                newName = recvName;
                do {
                    for (Light *act : moduls) {
                        if (act->getName() == newName) {
                            LOG_INFO("LIGHTserv:: Obdrzal som neplatne meno generujem nove");
                            if ((pos = recvName.find('-')) != std::string::npos){
                                newName = recvName.substr(0, pos);
                            }
                            newName += "-";
                            newName += to_string(counter);
                            counter++;
                            correctName = false;
                            break;
                        } else {
                            correctName = true;
                        }
                    }

                } while (!correctName);
                
                LOG_TRACE("LightServer:: pridelene meno ",newName); 
                aMutex->lock();
                moduls.push_back(new Light(socketClient, newName, conIsDisconnect, connectorToState));
                aMutex->unlock();
                LOG_INFO("LIGHTserv:: Light vytvorene");
            } else {
                LOG_INFO("LIGHTserv:: Nastala chyba pri inicializacii");
            }
        }
    }
    conIsDisconnect->signal();
}

void LightServer::threadControl() {
    aMutex->lock();
    while (*serverWork) {
        conIsDisconnect->wait(aMutex);
        for (int i = (moduls.size() - 1); i >= 0; i--) {
            if (!*serverWork) {
                delete moduls.at(i);
                moduls.erase(moduls.begin() + i);
            } else if (((Modul*) moduls.at(i))->isClientDisconnected()) {
                delete moduls.at(i);
                moduls.erase(moduls.begin() + i);
            }
        }
    }
    aMutex->unlock();
}

LightServer::~LightServer() {
    WaitForInternalThreadToExit();
    WaitForControlThreadToExit();
    delete serverSocket;
    delete conIsDisconnect;
    delete aMutex;
    LOG_WARN("LIGHTserv:: LightServer Destruktor vykonany");
}

