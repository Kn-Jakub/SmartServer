# SmartServer
Pri potrebné spustenia servisov cez LAMP:

    $ sudo /etc/init.d/mysql stop 
    $ sudo /etc/init.d/apache2 stop
    
Pri LAMP je potrebné urobiť linku do **/var/run/mysqld/mysqld.sock** z ** /opt/lampp/var/mysql/mysql.sock **
    cd /var/run
    sudo mkdir mysqld
    cd mysqld
    sudo ln -s /opt/lampp/var/mysql/mysql.sock ./mysqld.sock


Postup pri inštalácii:

    $ sudo apt-get install cmake
    $ cd /priecinok v ktorom su subory
    $ mkdir build
    $ cd build/
    $ cmake ../
    $ make -j4
    $ sudo make install
    $ ./smartServer
