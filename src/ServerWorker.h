/* 
 * File:   ServerWorker.h
 * Author: ripper
 *
 * Created on September 11, 2009, 12:31 PM
 */

#ifndef _SERVERWORKER_H
#define	_SERVERWORKER_H

#include <list>

#include "Runnable.h"
#include "Synchonizable.h"
#include "ServerWorkerMaster.h"
#include "Connection.h"

class ServerWorker : public Runnable, Synchonizable {
public:
    ServerWorker();
    ServerWorker(const ServerWorker& orig);
    virtual ~ServerWorker();

    virtual void addSocket(int fd) = 0;
    virtual void addConnection(Connection *connection) = 0;
    virtual void run() = 0;
    virtual int getConnectionCount() = 0;

    virtual void wait();
    virtual void notify();
};

#endif	/* _SERVERWORKER_H */

