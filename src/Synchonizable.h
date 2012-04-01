/* 
 * File:   Synchonizable.h
 * Author: ripper
 *
 * Created on September 17, 2009, 12:38 PM
 */

#ifndef _SYNCHONIZABLE_H
#define	_SYNCHONIZABLE_H

#include <pthread.h>
#include <time.h>

class Synchonizable {
public:
    Synchonizable();
    virtual ~Synchonizable();

    void wait();
    void notify();
private:
    Synchonizable(const Synchonizable& orig);

    pthread_mutex_t dataMutex;

    int synchronizingCounter;
    pthread_cond_t dataPresentCondition;
    struct timespec wait_time;
    bool firstTimeSync;
};

#endif	/* _SYNCHONIZABLE_H */

