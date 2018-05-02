/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MySQL.h
 * Author: Jakub Pekar
 *
 * Created on February 26, 2018, 8:06 PM
 */

#ifndef MYSQL_H
#define MYSQL_H
#include <mysql.h>
#include <string>
#include "../lbr_c++/definitions.h"

class MySQL {
private:
    MYSQL *connector;
   // string dbName;
public:
    MySQL();
    bool initMysql(const char* paDbName);
    bool createInitialStateTable();
    bool createStateTable(std::string tableName);
    bool createTable(std::string nameOfSensor);
    bool insertTo(std::string nameTable, TMPData paData);
    bool insertTo(std::string tableName, std::string sensorname, bool state,  bool lampOn, std::string light);
    bool insertTo(std::string tableName, std::string sensorname, bool state);
    bool setDefaultValue(uint16_t timePeriod,std::string logLevel,bool security);
    std::string getValue(std::string tableName);
    
    
    MySQL(const MySQL& orig) = delete;
    virtual ~MySQL();
private:

};

#endif /* MYSQL_H */

