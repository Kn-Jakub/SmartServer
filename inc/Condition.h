/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Thread.h
 * Author: JosoP
 *
 * Created on Utorok, 2017, decembra 19, 11:47
 */

#ifndef CONDITION_H
#define CONDITION_H

#include "Mutex.h"

class Condition{
private:
    pthread_cond_t pCondition;
public:
    Condition(){
        pthread_cond_init(&pCondition,NULL);
    }
    inline void signal(){
        pthread_cond_signal(&pCondition);
    }
    inline void wait(Mutex* mutex){
        pthread_cond_wait(&pCondition,mutex->getMutex());
    }
    /*inline void timedWait(Mutex* mutex){
        pthread_cond_timedwait(&pCondition,mutex->getMutex(),);
    }*/
    ~Condition() {
        pthread_cond_destroy(&pCondition);
    }
};


#endif /* CONDITION_H */

