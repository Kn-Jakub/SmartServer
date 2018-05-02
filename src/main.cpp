/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Jakub Pekar
 *
 */

#include "AppComunicator.h"
#include "../lbr_c++/definitions.h"
#include "Logger.h"
#include "globalData.h"
#include <unistd.h>
#include <cstdlib>
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    LOGGER_SET_LOG_LEVEL(LogLevelsEnum::TRACE);
    LOG_MESSAGE("***********************");
    LOG_MESSAGE("|Welcome on the server|");
    LOG_MESSAGE("***********************");
    LOG_MESSAGE("LOG list:");
    SERVER_DATA globalData;
    globalData.g_conStateDB = new MySQL();
    if (globalData.g_conStateDB->initMysql(DB_ACTIVE_MODULE)) {
        bool serverWork = true;
        bool alarmActive = false;
        uint16_t timeForLight = 10;
        globalData.g_conStateDB->setDefaultValue(timeForLight, "all", alarmActive);

        globalData.g_controlCondition = new Condition();
        globalData.g_serverWork = &serverWork;
        globalData.g_alarmActive = &alarmActive;
        globalData.timeForLight = &timeForLight;

        AppComunicator* aplication = new AppComunicator(&globalData);
        delete aplication;
        delete globalData.g_conStateDB;
        delete globalData.g_controlCondition;
        delete globalData.g_controlMutex;
    } else {
        LOG_FATAL("MAIN:::I can`t run the server, because it doesn`t run MySQL" );
    }
    return 0;
}

