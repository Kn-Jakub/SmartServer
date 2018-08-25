cmake_minimum_required(VERSION 3.0)
project(smartServerC++ VERSION "0.0.1")
set(PROJECT_DESCRIPTION "Server for monitoring object parameters")

message(STATUS "***" )
message(STATUS "*  " ${PROJECT_NAME})
message(STATUS "***" )

SET(publicHeaders
)

SET(privateHeaders
    inc/AppComunicator.h
    inc/Condition.h
    inc/DoorSensor.h
	inc/DoorServer.h
	inc/globalData.h
	inc/Light.h
	inc/LightServer.h
    	inc/Logger.h
	inc/Modul.h
	inc/Mutex.h
	inc/MVMSensor.h
	inc/MySQL.h
	inc/SecurityManager.h
	inc/SecurityServer.h
	inc/ServerExceptions.h
	inc/Socket.h
	inc/Thread.h
    inc/TmpSenzor.h
    inc/TmpServer.h
	lbr_c++/definitions.h
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

target_include_directories(${PROJECT_NAME} PUBLIC inc/public)
target_include_directories(${PROJECT_NAME} PRIVATE inc/)
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -Wall -I/usr/include/mysql  -Wfloat-conversion -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -lrt -ldl)
target_link_libraries(${PROJECT_NAME} mysqlclient -lpthread)

install(TARGETS ${PROJECT_NAME}
DESTINATION ${CMAKE_BINARY_DIR})
