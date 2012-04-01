/* 
 * File:   MainServerWorker.h
 * Author: ripper
 *
 * Created on September 11, 2009, 12:35 PM
 */

#ifndef _MAINSERVERWORKER_H
#define	_MAINSERVERWORKER_H

#include <sys/select.h>  /* for fd_set */
#include <list> /* for std::list */
#include <arpa/inet.h>   /* for sockaddr_in and inet_ntoa() */

#include "ServerWorker.h"
#include "config.h"
#include "ConnectionManager.h"
#include "Connection.h"
#include "DownloadManager.h"
#include "NewConnectionAccepter.h"

class MainServerWorker : public ServerWorker, public NewConnectionAccepter {
public:
    MainServerWorker(int port);

    virtual ~MainServerWorker();

    virtual void addSocket(int fd);
    virtual void addConnection(Connection *connection);
    virtual void run();

    virtual void wait();
    virtual void notify();

    virtual int getConnectionCount();
    virtual void addNewConnection(Connection *connection);
private:
    MainServerWorker(const MainServerWorker& orig);
    
    int listener;
    int server;
    int port;
    int fdmax;

    DownloadManager downloadManager;

    struct sockaddr_in serveraddr;
    std::list<ServerWorker *> workers;
    ConnectionManager connectionManager;

    ServerWorker *getOptimalWorker();

    

    void doBind();
    void doAccept();
    void addNewConnection(int fd);
};

#endif	/* _MAINSERVERWORKER_H */

