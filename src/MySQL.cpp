/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MySQL.cpp
 * Author: Jakub Pekar
 * 
 * Created on February 26, 2018, 8:06 PM
 */

#include <mysql/mysql.h>
#include <string.h>
#include <unistd.h>
#include "MySQL.h"
#include "Logger.h"

using namespace std;

MySQL::MySQL()
:connector(mysql_init(NULL))
{
}

bool MySQL::initMysql(const char* paDbName) {
    string address = "localhost";
    string user = "root";
    string password = "";
    if (mysql_real_connect(connector, address.c_str(), user.c_str(), password.c_str(), paDbName, 0, NULL, 0) == NULL) {
        LOG_WARN("MYSQL::Database ", paDbName, " is not created. Creating database");
        if (mysql_real_connect(connector, "localhost", "root", "", NULL, 0, NULL, 0) == NULL) {
            LOG_ERROR("MYSQL::Problem with connecting to MySQL (", mysql_error(connector), ")");
            return false;
        }
        string SQLCommand;
        SQLCommand += "CREATE DATABASE ";
        SQLCommand += paDbName;
        if (mysql_query(connector, SQLCommand.c_str())) {
            LOG_WARN("MYSQL:: Can`t create database ",string(paDbName)," (", mysql_error(connector), ")");
            return false;
        }
        //cout<< connector->db <<endl;
        LOG_INFO("MYSQL::Database ",string(paDbName) ," was create");
        mysql_select_db(connector, paDbName);
        //mysql_set_character_set(connector,"utf8");
        mysql_set_character_set(connector, "latin1_bin");
        if (string(paDbName) == string(DB_ACTIVE_MODULE)) {
            createStateTable(DBTAB_TEMP_MODULES);
            createStateTable(DBTAB_LIGHT_MODULES);
            createStateTable(DBTAB_MVM_MODULES);
        }
    } 
    LOG_INFO("MYSQL::Connector was succesfully connected to database: ", paDbName);
    return true;
}

bool MySQL::query(std::string& mysqlQuery) {
    if(mysql_query(connector, mysqlQuery.c_str())){
        LOG_ERROR("MYSQL:: can`t apply query", mysql_error(connector));
        return false;
    }
    LOG_TRACE("MYSQL:: ", mysqlQuery);
    return true;
}


bool MySQL::createInitialStateTable() {
    std::string SQLCommand;
    SQLCommand = "CREATE TABLE ";
    SQLCommand += "initialstate";
    SQLCommand += "(ID INT NOT NULL AUTO_INCREMENT, SvetloPerioda INT, LogLevel TEXT, SecurityState BOOLEAN,SecurityTime INT, ";
    SQLCommand += "PRIMARY KEY(ID))";
    LOG_TRACE("MYSQL:: ", SQLCommand);

    if (mysql_query(connector, SQLCommand.c_str())) {
        LOG_WARN("MYSQL:: can`t create table\n", mysql_error(connector));
        return false;
    }
    LOG_INFO("MYSQL:: Tabulka \"initialstate\" bola vytvorena ");

    return true;
}


bool MySQL::createStateTable(std::string tableName) {
    std::string SQLCommand;
    SQLCommand = "CREATE TABLE ";
    SQLCommand += tableName;
    SQLCommand += "(ID INT NOT NULL AUTO_INCREMENT, SensorName TEXT, State BOOLEAN, LastActivity TIMESTAMP, ";
    if (!tableName.compare(DBTAB_LIGHT_MODULES)) SQLCommand += "LightColor TEXT, LampOn BOOLEAN, AutoLight BOOLEAN, ";

    SQLCommand += "PRIMARY KEY(ID))";
    LOG_TRACE("MYSQL:: ", SQLCommand);

    if (mysql_query(connector, SQLCommand.c_str())) {
        LOG_WARN("MYSQL:: can`t create table\n", mysql_error(connector));
        return false;
    }
    LOG_INFO("MYSQL:: The table was create");

    return true;
}

bool MySQL::createTable(string nameOfSensor) {
    string SQLCommand;
    SQLCommand = "CREATE TABLE `" ;
    SQLCommand += nameOfSensor;
    SQLCommand += "` (ID INT NOT NULL AUTO_INCREMENT, Epoch TIMESTAMP, Temperature REAL, Humidity REAL, PRIMARY KEY(ID))";
    LOG_TRACE("MYSQL::", SQLCommand);

    if (mysql_query(connector, SQLCommand.c_str())) {
        LOG_ERROR("MYSQL:: can`t create table\n", mysql_error(connector));
        return false;
    }
    return true;
}

bool MySQL::setDefaultValue(uint16_t timePeriod, std::string logLevel, bool security,uint16_t securityTime) {
    string SQLCommand;
    MYSQL_RES *result;
    bool help = false;
    SQLCommand = "SELECT * FROM ";
    SQLCommand += "initialstate";
    do {
        LOG_DEBUG("MYSQL:: ", SQLCommand);
        if (mysql_query(connector, SQLCommand.c_str())) {
            LOG_WARN("MYSQL:: Table initialstate don`t exist");
            mysql_store_result(connector);
            createInitialStateTable();
            help = true;
        } else {
            result = mysql_store_result(connector);
            //mysql_free_result(result); 
            help = false;
        }

    } while (help);
    if (result->row_count) {
        SQLCommand = "UPDATE initialstate";
        SQLCommand += " SET SvetloPerioda = '";
        SQLCommand += to_string(timePeriod);
        SQLCommand += "', LogLevel = '";
        SQLCommand += logLevel;
        SQLCommand += security ? "', SecurityState = true, SecurityTime = '" : "', SecurityState  = false, SecurityTime = '";
        SQLCommand += to_string(securityTime) + "'";
        LOG_DEBUG("MYSQL:: ", SQLCommand);
        if (mysql_query(connector, SQLCommand.c_str())) {
            LOG_ERROR("MYSQL:: Problem with setting of state sensor \n(Error:)", mysql_error(connector));
            return false;
        }
        return true;
    } else {
        SQLCommand = "INSERT INTO initialstate";
        SQLCommand += " VALUES( NULL,'";
        SQLCommand += to_string(timePeriod);
        SQLCommand += "','";
        SQLCommand += logLevel;
        SQLCommand += string(security ? "',true,'" : "',false,'");
        SQLCommand += to_string(timePeriod) + "')";
        LOG_DEBUG("MYSQL:: ", SQLCommand);
        if (mysql_query(connector, SQLCommand.c_str())) {
            LOG_ERROR("MYSQL:: Problem with setting of state sensor\n(Error:)", mysql_error(connector));
            return false;
        }
        return true;
}
}


/*INSERT INTO TEMPERATURE TABLE*/
bool MySQL::insertTo(string nameTable, TMPData data) {
    string SQLCommand;
    SQLCommand += "INSERT INTO `";
    SQLCommand += nameTable;
    SQLCommand += "` VALUES(NULL, NOW(), ROUND(";
    SQLCommand += to_string(data.temperature);
    SQLCommand += ",3), ROUND(";
    SQLCommand += to_string(data.humidity);
    SQLCommand += ",3))";
    LOG_TRACE("MYSQL::", SQLCommand);
    if (mysql_query(connector, SQLCommand.c_str())) {
        LOG_ERROR("MYSQL::", mysql_error(connector));
        return false;
    }

    return true;
}
/*Insert to ligth state database*/
bool MySQL::insertTo(string tableName, string sensorname, bool state, bool lampOn, string light) {
    string SQLCommand;
    MYSQL_RES *result;
    bool help = false;
    SQLCommand = "SELECT * FROM ";
    SQLCommand += tableName;
    SQLCommand += " WHERE SensorName = '";
    SQLCommand += sensorname;
    SQLCommand += "'";
    do {
        LOG_DEBUG("MYSQL:: ", SQLCommand);
        if (mysql_query(connector, SQLCommand.c_str())) {
            LOG_WARN("MYSQL:: Table ", tableName, " don`t exist");
            mysql_store_result(connector);
            createStateTable(tableName);
            help = true;
        } else {
            result = mysql_store_result(connector);
            //mysql_free_result(result); 
            help = false;
        }

    } while (help);
    if (result->row_count) {
        SQLCommand = "UPDATE ";
        SQLCommand += tableName;
        SQLCommand += state ? " SET State = 1" : " SET State = 0";
        SQLCommand += ", LightColor = '";
        SQLCommand += light;
        SQLCommand += lampOn ? "', LampOn = 1" : "', LampOn = 0";

        SQLCommand += " WHERE SensorName = '";
        SQLCommand += sensorname;
        SQLCommand += "'";
        LOG_DEBUG("MYSQL:: ", SQLCommand);
        if (mysql_query(connector, SQLCommand.c_str())) {
            LOG_ERROR("MYSQL:: Problem with setting of state sensor:", sensorname, "\n(Error:)", mysql_error(connector));
            return false;
        }
        return true;
    } else {
        SQLCommand = "INSERT INTO ";
        SQLCommand += tableName;
        SQLCommand += " VALUES( NULL,'";
        SQLCommand += sensorname;
        SQLCommand += string(state ? "',true" : "',false");
        SQLCommand += ", NOW(),'";
        SQLCommand += light;
        SQLCommand += lampOn ? "',true" : "', false";
        SQLCommand += ",false)";
        LOG_DEBUG("MYSQL:: ", SQLCommand);
        if (mysql_query(connector, SQLCommand.c_str())) {
            LOG_ERROR("MYSQL:: Problem with setting of state sensor:", sensorname, "\n(Error:)", mysql_error(connector));
            return false;
        }
        return true;
    }
    //}

}

bool MySQL::insertTo(string tableName, string sensorname, bool state) {
    string SQLCommand;
    MYSQL_RES *result;
    bool help = false;
    SQLCommand = "SELECT * FROM ";
    SQLCommand += tableName;
    SQLCommand += " WHERE SensorName = '";
    SQLCommand += sensorname;
    SQLCommand += "'";
    do {
        LOG_DEBUG("MYSQL:: ", SQLCommand);
        if (mysql_query(connector, SQLCommand.c_str())) {
            LOG_WARN("MYSQL:: Table ", tableName, " don`t exist");
            mysql_store_result(connector);
            createStateTable(tableName);
            help = true;
        } else {
            result = mysql_store_result(connector);
            //mysql_free_result(result); 
            help = false;
        }
    } while (help);
    if (result->row_count) {
        SQLCommand = "UPDATE ";
        SQLCommand += tableName;
        SQLCommand += state ? " SET State = 1" : " SET State = 0";
        SQLCommand += " WHERE SensorName = '";
        SQLCommand += sensorname;
        SQLCommand += "'";
        LOG_DEBUG("MYSQL:: ", SQLCommand);
        if (mysql_query(connector, SQLCommand.c_str())) {
            LOG_ERROR("MYSQL:: Problem with setting of state sensor:", sensorname, "\n(Error:)", mysql_error(connector));
            return false;
        }
        return true;
    } else {
        SQLCommand = "INSERT INTO ";
        SQLCommand += tableName;
        SQLCommand += " VALUES( NULL,'";
        SQLCommand += sensorname;
        SQLCommand += string(state ? "',true" : "',false");
        SQLCommand += ", NOW())";

        LOG_DEBUG("MYSQL:: ", SQLCommand);
        if (mysql_query(connector, SQLCommand.c_str())) {
            LOG_ERROR("MYSQL:: Problem with setting of state sensor:", sensorname, "\n(Error:)", mysql_error(connector));
            return false;
        }
        return true;
    }
    //}

}

MySQL::~MySQL() {
    mysql_close(connector);
}

