/* 
 * File:   Runnable.h
 * Author: ripper
 *
 * Created on September 11, 2009, 12:41 PM
 */

#ifndef _RUNNABLE_H
#define	_RUNNABLE_H

class Runnable {
public:
    Runnable();
    virtual void run() = 0;


    virtual ~Runnable();
private:
    Runnable(const Runnable& orig);
};

#endif	/* _RUNNABLE_H */

