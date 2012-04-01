/* 
 * File:   ConnectionManager.cpp
 * Author: ripper
 * 
 * Created on September 23, 2009, 4:53 PM
 */

#include <stdlib.h>
#include <pthread.h>
#include <iostream>

#include "ConnectionManager.h"
#include "Connection.h"
#include "ProxyClientConnection.h"
#include "Logger.h"
#include "StringUtils.h"

ConnectionManager::ConnectionManager()  {

    FD_ZERO(&write_fds);
    FD_ZERO(&read_fds);

    connectionManagerMutex = PTHREAD_MUTEX_INITIALIZER;
    connectionSetMutex = PTHREAD_MUTEX_INITIALIZER;
}

ConnectionManager::ConnectionManager(const ConnectionManager& orig) {
    pthread_mutex_destroy(&connectionSetMutex);
    pthread_mutex_destroy(&connectionManagerMutex);
}

ConnectionManager::~ConnectionManager() {
}

fd_set ConnectionManager::getReadSet() {
    pthread_mutex_lock(&connectionManagerMutex);
    fd_set ret = read_fds;
    pthread_mutex_unlock(&connectionManagerMutex);
    return ret;
}

fd_set ConnectionManager::getWriteSet() {
    pthread_mutex_lock(&connectionManagerMutex);
    fd_set ret = write_fds;
    pthread_mutex_unlock(&connectionManagerMutex);
    return ret;
}

int ConnectionManager::getConnectionCount() {
    pthread_mutex_lock(&connectionSetMutex);
    int size = connections.size();
    pthread_mutex_unlock(&connectionSetMutex);
    return size;
}

void ConnectionManager::addExistingConnection(Connection* connection) {
    pthread_mutex_lock(&connectionSetMutex);
    int fd = connection->getFD();

    if (connections.find(fd) == connections.end()) {
        connections[fd] = connection;

        if (connection->isTimeToRead())
            FD_SET(fd, &read_fds);
        if (connection->isTimeToWrite())
            FD_SET(fd, &write_fds);
        }
    else {
        //TODO: add handle
        Connection *oldConnection = connections[fd];
        if (!oldConnection->isFinished())  {
            Logger::instance().log("Adding duplicate connection!", FATALERROR);
            pthread_mutex_unlock(&connectionSetMutex);
            throw 1;
            }
        else {
            FD_CLR(fd, &read_fds);
            FD_CLR(fd, &write_fds);
            
            delete oldConnection;
            connections[fd] = connection;
            
            if (connection->isTimeToRead())
                FD_SET(fd, &read_fds);
            if (connection->isTimeToWrite())
                FD_SET(fd, &write_fds);

            }
        }

    pthread_mutex_unlock(&connectionSetMutex);
}

void ConnectionManager::closeConnection(int fd) {
    pthread_mutex_lock(&connectionSetMutex);
    connections.erase(fd);
//    close(fd);
    FD_CLR(fd, &read_fds);
    FD_CLR(fd, &write_fds);
    pthread_mutex_unlock(&connectionSetMutex);
}

void ConnectionManager::addListener(int listener) {
    pthread_mutex_lock(&connectionManagerMutex);
    FD_SET(listener, &read_fds);
    pthread_mutex_unlock(&connectionManagerMutex);
}
/*
Connection *ConnectionManager::getConnection(int fd) {
    return connections[fd];
}
*/
void ConnectionManager::recieve(int fd) {
    pthread_mutex_lock(&connectionManagerMutex);
    
    pthread_mutex_lock(&connectionSetMutex);
    if (connections.find(fd) == connections.end()) return; // already deleted from set
    Connection *connection = connections[fd];
    pthread_mutex_unlock(&connectionSetMutex);
   
    try {
        connection->read();
    }
    catch (BasicException &e) {
        pthread_mutex_unlock(&connectionManagerMutex);
        throw;
    }



    if (!connection->isTimeToRead()) {
        FD_CLR(fd, &read_fds);
        }
    if (connection->isTimeToWrite()) {
        FD_SET(fd, &write_fds);
        }
    pthread_mutex_unlock(&connectionManagerMutex);
}

void ConnectionManager::write(int fd) {
    pthread_mutex_lock(&connectionManagerMutex);
    
    pthread_mutex_lock(&connectionSetMutex);
    if (connections.find(fd) == connections.end()) return; // already deleted from set
    Connection *connection = connections[fd];

    pthread_mutex_unlock(&connectionSetMutex);
   
    try {
        connection->write();
    }
    catch (BasicException &e) {
        pthread_mutex_unlock(&connectionManagerMutex);
        throw;
    }

    if (connection->isTimeToRead()) {
        FD_SET(fd, &read_fds);
        }
    if (!connection->isTimeToWrite()) {
        FD_CLR(fd, &write_fds);
        }

    pthread_mutex_unlock(&connectionManagerMutex);
}

void ConnectionManager::updateWritingState() {
    pthread_mutex_lock(&connectionManagerMutex);
    pthread_mutex_lock(&connectionSetMutex);

    std::list<int> toRemove;

    for (std::map<int,  Connection *>::iterator i = connections.begin(); \
            i != connections.end(); i++) {
                Connection *connection = (*i).second;
                int fd = connection->getFD();

                if (connection->isFinished()) {

                    FD_CLR(fd, &write_fds);
                    FD_CLR(fd, &read_fds);
                    //connections.erase(i);
                    //close(fd);
                    toRemove.push_back(fd);

                    //std::cout << "Removed connection " << fd << std::endl;
                    Logger::instance().log("Closed connection (fd="+stringOf(fd)+")", INFO);
                    }
                else {
					if (connection->isTimeToWrite()) 
						FD_SET(connection->getFD(), &write_fds);
					else
						FD_CLR(connection->getFD(), &write_fds);

					if (connection->isTimeToRead()) 
						FD_SET(connection->getFD(), &read_fds);
					else
						FD_CLR(connection->getFD(), &read_fds);


				}

//            std::cout << "Connection# " << fd << ": " << connection->isTimeToRead() << " " << connection->isTimeToWrite() << std::endl;
        }

    for (std::list<int>::iterator i = toRemove.begin(); i != toRemove.end(); i++)
        connections.erase(*i);

    pthread_mutex_unlock(&connectionSetMutex);
    pthread_mutex_unlock(&connectionManagerMutex);
}
