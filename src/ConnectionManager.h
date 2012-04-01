/* 
 * File:   ConnectionManager.h
 * Author: ripper
 *
 * Created on September 23, 2009, 4:53 PM
 */

#ifndef _CONNECTIONMANAGER_H
#define	_CONNECTIONMANAGER_H

#include <map>

#include "Connection.h"

class MainServerWorker;

class ConnectionManager {
public:
    ConnectionManager();

    virtual ~ConnectionManager();

    fd_set getReadSet();
    fd_set getWriteSet();
    int getConnectionCount();
    //void newConnection(int fd);
    //Connection *getConnection(int fd);
    void recieve(int fd);
    void write(int fd);
    void addListener(int fd);
    void closeConnection(int fd);
    void addExistingConnection(Connection *connection);
    void updateWritingState();
private:
    ConnectionManager(const ConnectionManager& orig);

    fd_set read_fds;
    fd_set write_fds;

    pthread_mutex_t connectionManagerMutex;
    pthread_mutex_t connectionSetMutex;
    std::map<int, Connection *> connections;
};

#endif	/* _CONNECTIONMANAGER_H */

