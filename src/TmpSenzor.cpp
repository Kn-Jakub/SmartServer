/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TmpSenzor.cpp
 * Author: Jakub Pekar
 * 
 * Created on Pondelok, 2017, decembra 18, 12:22
 */

#include "TmpSenzor.h"
#include "Socket.h"
#include "../lbr_c++/definitions.h"
#include "Logger.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <cstring>

TmpSenzor::TmpSenzor(Socket* clientSocket,std::string pName, Condition *paCondition, MySQL *paConnector,MySQL *paConnectToState, int paSerSocket2)
: connectorToTmpDb(paConnector),
connectorToStateDb(paConnectToState),
conIsDisconnect(paCondition),
sendPeriod(10),
clientSocket2(new Socket(PORT_TMP_2,paSerSocket2)){
    // Connecting of Second socket TMP
    modulSock = clientSocket;
    this->name = pName;
    clientDisconnected = false;
    StartInternalThread();
}

void TmpSenzor::threadMain() {
    
    char buffer[BUFF_LENGTH];
    string sendingBF;

    LOG_INFO("TMPSEN::(",name,")obdrzal som meno: ", name);
    
    buffer[0] = ANS_ACK;
    sendingBF = std::string(buffer);
    sendingBF += name;
    modulSock->send((uint8_t*)sendingBF.c_str(),sendingBF.size());
    
    LOG_INFO("TMPSEN:: Synchronizacia druheho socketu");
    clientSocket2->recieve(buffer,BUFF_LENGTH);
    buffer[0] = ANS_ACK;
//    sendingBF = std::string(buffer);
//    sendingBF += name;
    clientSocket2->send(buffer,sendingBF.size());
    /*Connecting of TmpSensor to the state database*/
     if (connectorToStateDb->insertTo(string(DBTAB_TEMP_MODULES),name,true)) {
        LOG_INFO("TMPSEN:: (" , name , ") Nastavenie stavu v state databaze");
    } else {
        LOG_INFO("TMPSEN:: (", name , ") Vytvorenie noveho riadku v state tabulke");
    }
    
    /*Connecting of TmpSenzor to the table of Temperature database*/
    if (connectorToTmpDb->createTable(name)) {
        LOG_INFO("TMPSEN:: (" , name , ") Pripojeny do existujucej databazy teplot");
    } else {
        LOG_INFO("TMPSEN:: (", name , ") tabulka teplot bola vytvorena");
    }

    while (!clientDisconnected) {
        string recvMsg;
        TMPData receivingData;
        if (modulSock->recieve(buffer, BUFF_LENGTH) > 0) {
            memcpy(&receivingData,buffer,sizeof(receivingData));
            connectorToTmpDb->insertTo(name,receivingData);
            
        } else {
            clientDisconnected = true;
        }

    }
    LOG_INFO("TMPSEN::(",name,")Klient sa odpojil vlakno sa  ukoncuje");
    delete clientSocket2;
    conIsDisconnect->signal();
}

void TmpSenzor::setPeriod(uint32_t period) {
    uint8_t buff[10];
    if(period){
        buff[0]=TMP_SET_PERIOD;
        memcpy(buff+1,&period,sizeof(period));
        clientSocket2->send(buff,5);
    } else {
    }
    
}



TmpSenzor::TmpSenzor(const TmpSenzor& orig)
: connectorToTmpDb(orig.connectorToTmpDb) {
    name = orig.name;
    connectorToStateDb = orig.connectorToStateDb;
    connectorToTmpDb = orig.connectorToTmpDb;
};

TmpSenzor& TmpSenzor::operator=(const TmpSenzor& right) {
    // Check for self-assignment!
    if (this == &right) // Same object?
        return *this; // Yes, so skip assignment, and just return *this.

    this->name = right.name;
    this->connectorToTmpDb = right.connectorToTmpDb;
    this->connectorToStateDb = right.connectorToStateDb;

    return *this;
}


TmpSenzor::~TmpSenzor() {
    delete modulSock;
    
    WaitForInternalThreadToExit();
     if (connectorToStateDb->insertTo(string(DBTAB_TEMP_MODULES),name,false)) {
        LOG_INFO("TMPSEN:: (" , name , ") Nastaveny stav neaktivity v state databaze(false)");
        
    } else {
        LOG_INFO("TMPSEN:: (", name , ") Some Error");
    }
    LOG_INFO("TMPSEN::(",name,")odstaneny z pamate");
    
}

