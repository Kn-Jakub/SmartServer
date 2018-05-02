/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Mutex.h
 * Author: JosoP
 *
 * Created on Utorok, 2017, decembra 19, 11:33
 */

#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

class Mutex {
private:
    pthread_mutex_t pMutex;
public:
    Mutex(){
        pthread_mutex_init(&pMutex,NULL);
    }
    inline void lock(){
        pthread_mutex_lock(&pMutex);
    }
    inline void tryLock(){
        pthread_mutex_trylock(&pMutex);
    }
    inline void unlock(){
        pthread_mutex_unlock(&pMutex);
    }
    pthread_mutex_t* getMutex() {
        return &pMutex;
    }

    ~Mutex() {
        pthread_mutex_destroy(&pMutex);
    }
};
#endif /* MUTEX_H */