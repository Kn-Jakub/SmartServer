/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Thread.h
 * Author: JosoP
 *
 * Created on Utorok, 2017, decembra 19, 16:57
 */

#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include "../lbr_c++/definitions.h"
class Thread
{
private:
    pthread_t thread;
    pthread_t threadControler;
    
    
public:
   Thread() {/* empty */}
   

   /** Returns true if the thread was successfully started, false if there was an error starting the thread */
   bool StartInternalThread()
   {
      return (pthread_create(&thread, NULL, InternalThreadEntryFunc, this) == 0);
   }
   bool StartControlThread()
   {
      return (pthread_create(&threadControler, NULL, ControlThreadEntryFunc, this) == 0);
   }

   /** Will not return until the internal thread has exited. */
   void WaitForInternalThreadToExit()
   {
      (void) pthread_join(thread, NULL);
   }
   void WaitForControlThreadToExit()
   {
      (void) pthread_join(threadControler, NULL);
   }
   
   ~Thread() {/* empty */}

protected:
   /** Implement this method in your subclass with the code you want your thread to run. */
   virtual void threadMain() {};
   virtual void threadControl() {};

private:
   static void * InternalThreadEntryFunc(void * This) {
       ((Thread *)This)->threadMain(); 
       return NULL;
   }
    static void * ControlThreadEntryFunc(void * This) {
       ((Thread *)This)->threadControl(); 
       return NULL;
   }
};

#endif /* THREAD_H */

