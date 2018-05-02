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

class TmpSenzor : public Thread , Modul {
private:
    MySQL *connectorToTmpDb;
    MySQL *connectorToStateDb;
    Condition *conIsDisconnect;
    unsigned int sendPeriod;
    Socket *servSocket2;
    Socket* clientSocket2;
public:
    TmpSenzor(Socket* socketDescriptor,
            std::string name, 
            Condition *paCondition, 
            MySQL *paConnector,
            MySQL *paConnectToState,
            int paSerSocket2);
    TmpSenzor& operator=(const TmpSenzor& right);
    
    string getName() const {    return name;    }
    void SetName(string name) { this->name = name;  }
    void setPeriod(uint32_t period);
    
    virtual ~TmpSenzor();
private:
    TmpSenzor(const TmpSenzor& orig);
    virtual void threadMain();
};

#endif /* TMPSENZOR_H */

