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
#include "Definitions.h"
#include <mysql.h>
#include <string>


class MySQL {
    private:
    MYSQL *connector;
public:
    MySQL();
    /**
    * Function initializes tho connector to the database
    * @param paDbName - string of database name
    * @return true if initialization is successful, false otherwise
    */
    bool initMysql(const char* paDbName);
    
    bool query(std::string& mysqlQuery);
    /**
    * Function create the state table of server
    * @return true if initialization is successful, false otherwise
    */
    bool createInitialStateTable();
    /**
    * Function create the state table of module
     * @param tableName - string of database name
    * @return true if creating is successful, false otherwise
    */
    bool createStateTable(std::string tableName);
    /**
    * Function create the temperature table for temperature module
     * @param nameOfSensor - string of table name
    * @return true if creating is successful, false otherwise
    */
    bool createTable(std::string nameOfSensor);
    /**
    * Function inserts the record to the temperature database
     * @param tableName- string of table name
     * @param pData- data entered into the database
     * @return true if inserting is successful, false otherwise
    */
    bool insertTo(std::string tableName, TMPData paData);
    bool insertTo(std::string tableName, std::string sensorname, bool state,  bool lampOn, std::string light);
    bool insertTo(std::string tableName, std::string sensorname, bool state);
    bool setDefaultValue(uint16_t timePeriod,std::string logLevel,bool security, uint16_t securityTime);
    std::string getValue(std::string tableName);
    
    MySQL(const MySQL& orig) = delete;
    virtual ~MySQL();


};

#endif /* MYSQL_H */

