/* 
 * File:   CasualServerWorker.h
 * Author: ripper
 *
 * Created on September 17, 2009, 12:29 PM
 */

#ifndef _CASUALSERVERWORKER_H
#define	_CASUALSERVERWORKER_H

#include <pthread.h>

#include "ServerWorker.h"
#include "Connection.h"
#include "ConnectionManager.h"
#include "MainServerWorker.h"
#include "DownloadManager.h"

class CasualServerWorker : public ServerWorker {
public:
    CasualServerWorker(DownloadManager *downloadManager);
    CasualServerWorker(const CasualServerWorker& orig);
    virtual ~CasualServerWorker();

    virtual void addSocket(int fd);
    virtual void run();
    virtual int getConnectionCount();
    virtual void addConnection(Connection *connection);

private:
    pthread_t thread;
    bool hasNoWork;

    DownloadManager *downloadManager;

    int fdmax;
    ConnectionManager connectionManager;
};

#endif	/* _CASUALSERVERWORKER_H */

