/* 
 * File:   Synchonizable.cpp
 * Author: ripper
 * 
 * Created on September 17, 2009, 12:38 PM
 */

#include "Synchonizable.h"
#include "config.h"

#include <iostream>
#include <stdio.h> /* for printf */
#include <errno.h>

Synchonizable::Synchonizable() {
    dataMutex = PTHREAD_MUTEX_INITIALIZER;
    dataPresentCondition = PTHREAD_COND_INITIALIZER;

    synchronizingCounter = 0;

    firstTimeSync = true;

}

Synchonizable::Synchonizable(const Synchonizable& orig) {
    throw 1;
}

Synchonizable::~Synchonizable() {
   //TODO: add return value check
   pthread_mutex_destroy(&dataMutex);
   pthread_cond_destroy(&dataPresentCondition);
}



void Synchonizable::wait() {
    int rc;

    if (pthread_mutex_lock(&dataMutex)) {
        printf("Mutex lock failed\n");
        //TODO: throw exception
        }


    //if (!firstTimeSync) {
    //if (true) {
        wait_time.tv_sec = time(NULL) + THREAD_WAIT_TIME;
        wait_time.tv_nsec = 0;

        rc = pthread_cond_timedwait(&dataPresentCondition, &dataMutex, &wait_time);

    //    if (!rc) {
    //        firstTimeSync = false;
    //        }
    //    }

    if (pthread_mutex_unlock(&dataMutex)) {
        printf("Mutex unlock failed\n");
        //TODO: throw exception
        }

    //else
    //    rc = pthread_cond_wait(&dataPresentCondition, &dataMutex);
}

void Synchonizable::notify() {
    int rc;
    //std::cout << "Synchonizable::notify() begin" << std::endl;


    if (pthread_mutex_lock(&dataMutex)) {
        printf("Mutex lock failed\n");
        //TODO: throw exception
        return;
        }

    if ((rc = pthread_cond_signal(&dataPresentCondition))) {
        //TODO: throw exception
        printf("Condsignal failed, rc=%d\n", rc);
        pthread_mutex_unlock(&dataMutex);
        }

    if (pthread_mutex_unlock(&dataMutex)) {
        printf("Mutex lock failed\n");
        //TODO: throw exception
        }
    
    //std::cout << "Synchonizable::notify() end" << std::endl;
}
