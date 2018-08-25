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
public:
    Condition(){
        pthread_cond_init(&pCondition,NULL);
    }
    ~Condition() {
        pthread_cond_destroy(&pCondition);
    }
     /**
    * Function send signal into lock mutex
    */
    inline void signal(){
        pthread_cond_signal(&pCondition);
    }
    /**
    * Function set mutex to waiting state
     * @param mutex - is object which represent Mutex
    */
    inline void wait(Mutex* mutex){
        pthread_cond_wait(&pCondition,mutex->getMutex());
    }
private:
    pthread_cond_t pCondition;    
    
};

#endif /* CONDITION_H */

