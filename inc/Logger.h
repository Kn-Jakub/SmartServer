/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Logger.h
 * Author: Jakub Pekar
 *
 * Created on February 28, 2018, 12:35 PM
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <sstream>
#include <ctime>
#include <string>

//#ifndef LOG_LEVEL
//#define LOG_LEVEL LogLevelsEnum::ALL
//#endif
#define LOG_MESSAGE(...) \
    Logger::instance().writeMessage(__VA_ARGS__)
#define LOG_ALL(...)  \
    Logger::instance().log(LogLevelsEnum::ALL, " all ", time(nullptr), __VA_ARGS__)
#define LOG_TRACE(...)  \
    Logger::instance().log(LogLevelsEnum::TRACE, "trace", time(nullptr), __VA_ARGS__)
#define LOG_DEBUG(...)  \
    Logger::instance().log(LogLevelsEnum::DEBUG, "debug", time(nullptr), __VA_ARGS__)
#define LOG_INFO(...)  \
    Logger::instance().log(LogLevelsEnum::INFO, "info ", time(nullptr), __VA_ARGS__)
#define LOG_WARN(...)  \
    Logger::instance().log(LogLevelsEnum::WARN, "warni", time(nullptr), __VA_ARGS__)
#define LOG_ERROR(...)  \
    Logger::instance().log(LogLevelsEnum::ERROR, "error", time(nullptr), __VA_ARGS__)
#define LOG_FATAL(...)  \
    Logger::instance().log(LogLevelsEnum::FATAL, "fatal", time(nullptr), __VA_ARGS__)
#define LOGGER_SET_LOG_LEVEL(...) \
    Logger::instance().setLogLevel(__VA_ARGS__)

namespace {

    template<typename T_head>
    void Log(std::stringstream& ss, T_head& head) {
        ss << head;
    }

    template<typename T_head, typename... T_tail>
    void Log(std::stringstream& ss, T_head& head, T_tail&... tail) {
        Log(ss, head);
        Log(ss, tail...);
    }

    template<>
    void Log<uint8_t>(std::stringstream& ss, uint8_t& head) {
        ss << std::to_string((int) head);
    }

    template<>
    void Log<bool>(std::stringstream& ss, bool& head) {
        if (head) {
            ss << "true";
        } else {
            ss << "false";
        }
    }
}

enum LogLevelsEnum {
    ALL = 0,
    TRACE = 1,
    DEBUG = 2,
    INFO = 3,
    WARN = 4,
    ERROR = 5,
    FATAL = 6
};

class Logger {
private:
    int logLevelFilter;

    Logger() {
        logLevelFilter = LogLevelsEnum::ALL;
    }
public:

    static Logger& instance() {
        static Logger INSTANCE;
        return INSTANCE;
    }

    Logger(const Logger&) = delete;
    void operator=(Logger const&) = delete;
    ~Logger() = default;

    /**
     * Function for writing logs to cout
     * @param logLevel - level of log message
     * @param level - level in string form
     * @param time - time when the log was create
     * @param logs - it is message from macro
     */

    template<typename... T>
    void log(LogLevelsEnum logLevel, std::string level, time_t time, T... logs) {
        std::string logMessage;
        std::stringstream ss;
        if (logLevel >= logLevelFilter) {
            Log(ss, logs...);
            struct tm tm = *localtime(&time);
            if(tm.tm_mday < 10){
                logMessage = std::string("0") + std::to_string(tm.tm_mday);   
            } else {
                logMessage = std::to_string(tm.tm_mday);
            }
            
            if(tm.tm_mon < 10){
                logMessage += "/0" + std::to_string(tm.tm_mon +1);
            } else {
                logMessage += "/" + std::to_string(tm.tm_mon+1);
            }
            
            logMessage += "/" + std::to_string(tm.tm_year +1900) + " ";
            
            if(tm.tm_hour < 10){
                logMessage += std::to_string(tm.tm_hour);
            } else {
                logMessage += std::to_string(tm.tm_hour);
            }
            
            if(tm.tm_min < 10){
                logMessage += ":0" + std::to_string(tm.tm_min);
            } else {
                logMessage += ":" + std::to_string(tm.tm_min);
            }
             
             if(tm.tm_sec < 10){
                logMessage += ":0" + std::to_string(tm.tm_sec);
            } else {
                logMessage += ":" + std::to_string(tm.tm_sec);
            }

            logMessage += " [";
            logMessage += level;
            logMessage += "] ";
            logMessage += ss.str();

            std::cout << logMessage << std::endl;
        }
    }
    /**
     * Function for writing message to cout, without log level filtering
     * @param message - it is message from macro
     */
    template<typename... T>
    void writeMessage(T... message) {
        std::stringstream ss;
        Log(ss, message...);
        std::cout << ss.str() << std::endl;
    }
     /**
     * Function set the maximal log level for logs filtering
     * @param message - it is message from macro
     */
    template<typename... T>
    void setLogLevel(T... logs) {

        std::stringstream ss;
        Log(ss, logs...);
        uint8_t logLvl = std::stoi(ss.str().c_str());

        this->logLevelFilter = (int) logLvl;
    }

};

#endif /* LOGGER_H */

