/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TmpSenzor.h
 * Author: Jakub Pekar
 *
 * Created on Pondelok, 2017, decembra 18, 12:22
 */




#ifndef TMPSENZOR_H
#define TMPSENZOR_H

#include <string>
#include <vector>
#include "Thread.h"
#include "Modul.h"
#include "../lbr_c++/definitions.h"
#include "Condition.h"
#include "MySQL.h"

using namespace std;

class TmpSenzor : public Thread, Modul {
public:
    TmpSenzor(Socket* socketDescriptor,
            std::string name,
            Condition *paCondition,
            MySQL *paConnector,
            MySQL *paConnectToState,
            int paSerSocket2);
    virtual ~TmpSenzor();
    /**
    * Function returns the name of sensor
    * @return string name of sensor
    */
    string getName() const {return name;}
    /**
    * Function set the name sensor
    * @param name - setting name
    */
    void SetName(string name) {this->name = name;}
    /**
    * Function set the period of sending data from module 
    * @param period - period sending data
    */
    void setPeriod(uint32_t period);
    TmpSenzor& operator=(const TmpSenzor& right);

private:
    TmpSenzor(const TmpSenzor& orig);
    virtual void threadMain();
private:
    MySQL *connectorToTmpDb;
    MySQL *connectorToStateDb;
    Condition *conIsDisconnect;
    unsigned int sendPeriod;
    Socket* clientSocket2;
};

#endif /* TMPSENZOR_H */

