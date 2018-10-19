/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GlobalData.h
 * Author: jakub
 *
 * Created on March 26, 2018, 9:08 PM
 */

#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include "Mutex.h"
#include "Condition.h"
#include "MySQL.h"


typedef struct ServerGlobalData{
    bool *g_serverWork;
    bool *g_alarmActive;
   // Mutex *g_controlMutex;
   // Condition *g_controlCondition;
    MySQL *g_conStateDB;
    uint16_t* timeForLight;
    
}SERVER_DATA;


#endif /* GLOBALDATA_H */

