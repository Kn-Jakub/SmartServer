# SmartServer (C++ Application)

## About

This project is server application for my Bachalor's project.

## Preparing for installation

This settings are neccessary for installation and run the application.

    $ sudo apt-get update
    $ sudo apt-get install apache2 mysql-server mysql-client
    $ sudo apt-get install libmysqlclient-dev

## Installation

    $ mkdir build
    $ cd build/
    $ cmake ..
    $ make -j4
    $ sudo make install
    $ ./smartServer

