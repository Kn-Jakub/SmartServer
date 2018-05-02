/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Light.cpp
 * Author: Jakub Pekar
 * 
 * Created on Streda, 2018, januára 3, 13:54
 */

#include <iostream>
#include <algorithm>
#include <unistd.h>

#include "Light.h"
#include "Socket.h"
#include "Condition.h"
#include "Logger.h"

Light::Light(Socket *socketClient,string recvName, Condition *paConIsDisconnect, MySQL *paConnectorState)
:connectorToState(paConnectorState) {
    name = recvName;
    clientDisconnected = false;
    conIsDisconnect = paConIsDisconnect;
    lightOn = false;
    modulSock = socketClient;
    StartInternalThread();
}

void Light::threadMain() {
    char buffer[BUFF_BIG_LENGTH];
    string recv;
    /*Receiving of name*/
    recv = std::string(1,ANS_ACK);
    recv += name;
    modulSock->send((uint8_t*) recv.c_str(), recv.size());
    if (true) {

    } else {
        LOG_WARN("LIGHT:: Error inicializacie klienta");
    }
    /*Receiving of default state from light*/
    if (modulSock->recieve(buffer, BUFF_BIG_LENGTH) > 0) {
        if (buffer[1] == 1) {
            lightOn = true;
        } else {
            lightOn = false;
        }

        lightColor = string(buffer + 2, 6);
    }

    LOG_INFO("LIGHT:: (", name, ") obdrzal defualt state: ", lightColor);

    /*Connecting of TmpSensor to the state database*/
    if (connectorToState->insertTo(string(DBTAB_LIGHT_MODULES), name, true, lightOn, lightColor)) {
        LOG_DEBUG("LIGHT:: (", name, ") Nastavujem stav v State databaze");
    } else {
        LOG_DEBUG("LIGHT:: (", name, ") Vytvaram novu tabulku");
    }
    while (!clientDisconnected) {
        if (modulSock->recieve(buffer, BUFF_LENGTH) > 0) {

        } else {
            clientDisconnected = true;
        }

    }
    LOG_INFO("LIGHT:: (", name, ") Klient odpojeny");
    conIsDisconnect->signal();
}

void Light::setStateOfLight( bool pLightOn, string pLightColor) {

    string sendBuffer(1, (char) LIGHT_STATE_MESSAGE);
    lightOn = pLightOn;
    sendBuffer += lightOn ? string(1, 1) : string(1, 0);
    if(pLightColor == " "){
        sendBuffer += lightColor;
    } else {
        sendBuffer += pLightColor;
        this->lightColor = pLightColor;
    }
    LOG_INFO("LIGHT:: (", name, ") Nastavujem stav na: ", pLightOn, "  ", pLightColor);
    LOG_DEBUG("LIGHT:: (", name, ")Odosielane DATA: ", sendBuffer.c_str());
    modulSock->send((char*) sendBuffer.c_str(), sendBuffer.size());

}

bool Light::sendAlert() {
    string sendBuffer(1, (char) LIGHT_ALERT);
    LOG_WARN("LIGHT:: (", name, ") *ALARM ACTIV ALERT*");
    modulSock->send((char*) sendBuffer.c_str(), sendBuffer.size());
    //modulSock->recieve(buffer, BUFF_LENGTH);
    return true;
}
bool Light::setAlarm(bool start) {
    string sendBuffer;
    if(start){
        sendBuffer = string(1, (char) LIGHT_ALARM);
        LOG_WARN("LIGHT:: (", name, ") *ALARM ACTIV*");
    }else {
        sendBuffer = string(1, (char) LIGHT_ALARM_STOP);
        LOG_WARN("LIGHT:: (", name, ") *ALARM STOP*");
    }
    modulSock->send((char*) sendBuffer.c_str(), sendBuffer.size());
    return true;
}

Light::~Light() {
    WaitForInternalThreadToExit();
    if (connectorToState->insertTo(string(DBTAB_LIGHT_MODULES), name, false, lightOn, lightColor)) {
        LOG_DEBUG("LIGHT:: (", name, ")nastaveny stav v Database(false)");

    } else {
        LOG_DEBUG("LIGHT:: (", name, ") Some Error with set of exit stete to DB");
    }
    delete modulSock;
    LOG_WARN("LIGHT:: (", name, ") Odstranene z pamate");
}

