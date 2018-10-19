cmake_minimum_required(VERSION 3.0)
project(smartServerC++ VERSION "0.0.1")
set(PROJECT_DESCRIPTION "Server for monitoring object parameters")

message(STATUS "***" )
message(STATUS "*  " ${PROJECT_NAME})
message(STATUS "***" )

SET(publicHeaders
)

SET(privateHeaders
    include/AppComunicator.h
    include//Condition.h
    include/DoorSensor.h
    include/DoorServer.h
    include/GlobalData.h
    include/Light.h
    include/LightServer.h
    include/Logger.h
    include/Modul.h
    include/Mutex.h
    include/MVMSensor.h
    include/MySQL.h
    include/SecurityManager.h
    include/SecurityServer.h
    include/Socket.h
    include/Thread.h
    include/TmpSenzor.h
    include/TmpServer.h
    include/Definitions.h
)

SET(privateSources
    src/AppComunicator.cpp
    src/DoorSensor.cpp
    src/DoorServer.cpp
    src/Light.cpp
    src/LightServer.cpp
    src/MVMSensor.cpp
    src/MySQL.cpp
    src/SecurityManager.cpp
    src/SecurityServer.cpp
    src/Socket.cpp
    src/TmpSenzor.cpp
    src/TmpServer.cpp
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${privateSources} ${privateHeaders} ${publicHeaders})

target_include_directories(${PROJECT_NAME} PUBLIC include/public)
target_include_directories(${PROJECT_NAME} PRIVATE include/)
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -I/usr/include/mysql  -Wfloat-conversion -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -lrt -ldl)
target_link_libraries(${PROJECT_NAME} mysqlclient -lpthread)

install(TARGETS ${PROJECT_NAME}
DESTINATION ${CMAKE_BINARY_DIR})
