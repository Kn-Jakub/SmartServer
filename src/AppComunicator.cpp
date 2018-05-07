 /*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AppComunicator.cpp
 * Author: Jakub Pekar
 * 
 */
#include "Logger.h"
#include "AppComunicator.h"
#include "SecurityManager.h"
#include <unistd.h>
#include <string>

AppComunicator::AppComunicator(SERVER_DATA *pServerData)
: serverWork(pServerData->g_serverWork),
        lightServer(new LightServer(serverWork, pServerData->g_conStateDB)),
tmpServer(new TmpServer(serverWork, pServerData->g_conStateDB)),
alarmRunCondition(new Condition()),
securityManager(new SecurityManager(serverWork,pServerData->g_alarmActive,lightServer,pServerData->g_conStateDB,alarmRunCondition)),
secServer(new SecurityServer(serverWork,alarmRunCondition, pServerData->g_alarmActive, lightServer, pServerData->timeForLight, pServerData->g_conStateDB)),
doorServer(new DoorServer(serverWork, pServerData->g_alarmActive, alarmRunCondition, pServerData->g_conStateDB)),
webServerSocket(new Socket(PORT_WEB_SERVER)),
webServerConnect(true),
serverData(pServerData)
{  
    name = "WebServer";
    LOG_DEBUG("APPCom:: Starting thread");
    StartInternalThread();
}
/*
 * Hlavna funkcia vlakna v ktorej prebieha hlavna komunikacia s webServerom
 */
void AppComunicator::threadMain() {
    
    int returnValue;
    int clientSocket;
    char bufferBig[BUFF_BIG_LENGTH];
    webServerConnect = false;
    size_t pos;
    bool lampOn;
    string recvMsg;
    string data;
    string delimeter = "@";
    std::string SQLQuery;
  
    webServerSocket->serverListen();
    
    while (*serverWork) {
        clientSocket = webServerSocket->connectClient();
        modulSock = new Socket(PORT_WEB_SERVER, clientSocket);
        LOG_INFO("APPCom:: Cakam na ziadost od uzivatela");
        bufferBig[0] = 0;
        returnValue = modulSock->recieve(bufferBig, BUFF_BIG_LENGTH);
        if (returnValue <= 0) {
            LOG_INFO("APPCom::", name, " Odpojena  ", modulSock->getSocket());
            delete modulSock;
        } else {
            bufferBig[returnValue] = 0;
            data = string(bufferBig+2);
            LOG_DEBUG("APPCom::Obdrzane data z webservera", data);
            LOG_DEBUG("APPCom::Volba ", (int) bufferBig[0]);

            switch (bufferBig[0]) {
                case REQV_SET_LIGHT_STATE:
                    recvMsg = string(bufferBig + 2); 
                    /* Message format:   [TypeMSG(1)] [bool Light(1)] [COlor(6)] [Name]*****/
                    data = recvMsg.substr(0,6);
                    recvMsg.erase(0, 6);
                    lampOn = (bufferBig[1] == '1') ? true : false;
                    secServer->removeConnection(recvMsg);
                    lightServer->setLight(data, lampOn, recvMsg);
                    break;
                case REQV_SET_TMP_PERIOD:
                    /*Message format:    [TypeMSG(1)] [uint16 timePeriod] [Name]*/
                    uint16_t timeTmp;
                    memcpy(&timeTmp,bufferBig + 1,sizeof(uint16_t));
                    recvMsg =string(bufferBig + 3);
                    LOG_INFO("APPCom::Nastavujem periodu pre teplotny senzor: ", to_string(timeTmp));
                    tmpServer->setPeriodOnSensor(recvMsg,timeTmp);
                    
                    break;
                case REQV_SET_LOG_LEVEL:
                    recvMsg = string(bufferBig + 3);
                    SQLQuery = "UPDATE initialstate SET LogLevel = '" + recvMsg + "', SvetloPerioda = '";
                    LOG_MESSAGE("APPCom::Nastavujem Log Level na: ",recvMsg);
                    if (recvMsg == "all"){
                        LOGGER_SET_LOG_LEVEL(LogLevelsEnum::ALL);
                    } else if (recvMsg == "trace"){
                        LOGGER_SET_LOG_LEVEL(LogLevelsEnum::TRACE);
                    } else if (recvMsg == "debug"){
                        LOGGER_SET_LOG_LEVEL(LogLevelsEnum::DEBUG);
                    }  else if (recvMsg == "info"){
                        LOGGER_SET_LOG_LEVEL(LogLevelsEnum::INFO);
                    } else if (recvMsg == "warning"){
                        LOGGER_SET_LOG_LEVEL(LogLevelsEnum::WARN);
                    }  else if (recvMsg == "error"){
                        LOGGER_SET_LOG_LEVEL(LogLevelsEnum::ERROR);
                    }  else if (recvMsg == "fatal"){
                        LOGGER_SET_LOG_LEVEL(LogLevelsEnum::FATAL);
                    }
                    memcpy(serverData->timeForLight,bufferBig + 1,sizeof(uint16_t));
                    SQLQuery += to_string(*serverData->timeForLight) + "'";
                    LOG_INFO("APPCom::Nastavujem periodu pre osvetlenie: ", to_string(*serverData->timeForLight));
                    serverData->g_conStateDB->query(SQLQuery);
                    break;
                case REQV_ALARM_CONTROL:
                    if(bufferBig[1] == '1'){
                        *serverData->g_alarmActive = true;
                    } else {
                        *serverData ->g_alarmActive = false;
                        alarmRunCondition->signal();
                    }
                    
                    uint16_t timeAlarmALert;
                    memcpy(&timeAlarmALert,bufferBig + 2,sizeof(uint16_t));
                    SQLQuery = "UPDATE initialstate SET SecurityState = ";
                    SQLQuery += *serverData->g_alarmActive ? "true" : "false";
                    SQLQuery += " ,SecurityTime = '" + to_string(timeAlarmALert)+ "'";
                    securityManager->setReactionTime(timeAlarmALert);
                    LOG_WARN("APPCom::Nastavujem alarmActive na ",*serverData->g_alarmActive ? " aktivne" : " neaktivne"," a reakcny cas: ", to_string(timeAlarmALert));
                    serverData->g_conStateDB->query(SQLQuery);
                    break;
                case REQV_SHUTDOWN_SERVER: //******************************************************************
                    LOG_WARN("APPCom::Pouzivatel vypina aplikaciu aj server");
                    shutDown();

                    break;
                case REQV_ASSIGNED_MVMTOLIGHT: //******************************************************************
                    LOG_DEBUG("APPCom::Pouzivatel chce priradit svetlo k senzoru");
                    recvMsg = string(bufferBig + 1);
                    pos = recvMsg.find(delimeter);
                    data = recvMsg.substr(0, pos);
                    recvMsg.erase(0, pos + 1);
                    if(secServer->addConection(data,recvMsg)){
                        LOG_DEBUG("APPCom:: Svetlo uspesne priradene");
                    } else {
                        LOG_DEBUG("APPCom:: Nastala chyba pri priradovani, svetlo neexistuje");
                    }
                    break;    

                default: LOG_WARN("APPCom:: Nespravny REQV");
                    break;
            }
        }
    }

}
/*
 * Metoda pre bezpecne vypnutie servera
 */
void AppComunicator::shutDown() {
    Socket* shutDownTMP;
    Socket* shutDownLight;
    *serverWork = false;
    shutDownTMP = new Socket(PORT_TMP, (char*) "localhost", &webServerConnect);
    shutDownLight = new Socket(PORT_ACTUATOR, (char*) "localhost", &webServerConnect);
    delete shutDownLight;
    delete shutDownTMP;

}

AppComunicator::~AppComunicator() {
    WaitForInternalThreadToExit();
    LOG_INFO("APPCom:: Volam Delete lightServer");
    delete lightServer;
    LOG_INFO("APPCom:: Volam Delete lightServer");
    delete secServer;
    LOG_INFO("APPCom:: Volam Delete lightServer");
    delete lightServer;
    LOG_INFO("APPCom:: Volam Delete tmpServer");
    delete tmpServer;
    LOG_INFO("APPCom:: Volam Delete ServerSocket");
    delete webServerSocket;
}

