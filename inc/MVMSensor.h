/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MVMSensor.h
 * Author: jakub
 *
 * Created on March 19, 2018, 7:21 PM
 */

#ifndef MVMSENSOR_H
#define MVMSENSOR_H

#include "LightServer.h"
#include "MySQL.h"
#include "Condition.h"
#include "Socket.h"
#include "Thread.h"
#include "Modul.h"

#include <vector>

class MVMSensor : public Thread, Modul {
public:
    MVMSensor(Socket* socketDescriptor,
            std::string pName,
            bool *pAlarmActive,
            uint16_t* pTimeForLight,
            LightServer *pLightServer,
            Condition *alarm,
            Condition *paCondition,
            MySQL *paConnectToState);
    virtual ~MVMSensor();

    /**
     * Function returns name of module
     * @return const string name of module
     */
    string getName() const {
        return name;
    }
    /**
    * Function adds name of ligth which will be switched by this sensor
    * @param light - name of light
    * @return true if set is successful, false otherwise
    */
    void addLight(std::string light);
    /**
    * Function removes name of ligth from vector, which will be switched by this sensor
    * @param light - name of light
    * @return true if set is successful, false otherwise
    */
    bool removeLight(std::string& pName);
    MVMSensor(const MVMSensor& orig) = delete;
private:
    virtual void threadMain();
private:
    MySQL *m_connectorToStateDB;
    Condition *conIsDisconnect;
    std::vector<std::string> controledLights;
    LightServer *m_lightServer;
    Condition *alarm;
    uint16_t *timeForLight;
    bool *alarmActive;

};

#endif /* MVMSENSOR_H */

