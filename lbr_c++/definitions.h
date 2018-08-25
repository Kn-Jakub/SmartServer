/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   definitions.h
 * Author: Jakub Pekar
 *
 * Created on Pondelok, 2017, decembra 18, 9:30
 */

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/time.h>
#include <cstdint>
    
/******** PORT --- Definitions*******/  
#define PORT_TMP            10006
#define PORT_TMP_2          10007
#define PORT_ACTUATOR       10005
#define PORT_DOOR_SENZOR    10008    
#define PORT_MVM_SENZOR     10009
#define PORT_WEB_SERVER     10010
   
#define WEBSERVER "localhost"    

    /******** Internal Definitions ******/
#define BUFF_BIG_LENGTH         255
#define BUFF_LENGTH             100
#define COUNT_OF_LIGHTS         4
#define SIZE_OF_REOCRDS         50    
#define INIT                    112
    /******  Comunication mesgType from WebServer  *******/
#define REQV_SET_LIGHT_STATE    0
#define REQV_SET_TMP_PERIOD     1
#define REQV_SHUTDOWN_SERVER    2  
#define REQV_ASSIGNED_MVMTOLIGHT    3
#define REQV_ALARM_CONTROL      4   
#define REQV_SET_LOG_LEVEL      5
#define REQV_SET_LIGHT_PERIOD   6
    
    /***** Comunication mesgType from LigthSensor  ******/
#define LIGHT_STATE_MESSAGE     111
#define LIGHT_INIT              112
#define LIGHT_ALERT             113
#define LIGHT_ALARM             114 
#define LIGHT_ALARM_STOP        115

   /***** Comunication mesgType from TMPSensor  ******/
#define TMP_MESSAGE             101
#define TMP_SET_PERIOD          102
#define TMP_STOP_SENDING        103
    /***** Comuniication message type DOOR*/
#define MVM_DOOR_OPEN           120
#define MVM_DOOR_CLOSED         121    
    
   /***** Comunication mesgType from MVMSensor  ******/
#define MVM_SIGNAL          130
#define MVM_SIGNAL_ACK      130  
#define MVM_STOP            131
#define ANS_ACK                 20
    
#define OK                      10
/*Database dafine constant*/
#define DB_ACTIVE_MODULE "activemodules"
#define DBTAB_LIGHT_MODULES  "lightmodules"
#define DB_SERVER_TEMPERATURE "servertemperatures"
#define DBTAB_TEMP_MODULES "temperaturemodules" 
#define DBTAB_MVM_MODULES "mvmmodules"
    
//#define END_OF_DATA         100
//#define MODUL_NOT_EXIST     101
//#define NO_MODULE           102
//#define DATA_OK             103

//server exceptions
#define EX_PORT_WRONG_NUMBER    1
#define EX_SOCK_CREATING        2
#define EX_BIND                 3
#define EX_ACCEPT               4
#define EX_SOCK_ACCEPT          5


typedef struct TMPData{
    uint8_t type;
    double temperature;
    double humidity;
}TMPData;

#ifdef __cplusplus
}
#endif

#endif /* DEFINITIONS_H */

