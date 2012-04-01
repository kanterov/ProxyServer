/* 
 * File:   CasualServerWorker.cpp
 * Author: ripper
 * 
 * Created on September 17, 2009, 12:29 PM
 */

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <iostream>

#include "CasualServerWorker.h"
#include "ProxyClientConnection.h"
#include "Logger.h"
#include "ServerException.h"
#include "StringUtils.h"

static void *serverWorkerRunThread(void *worker);

CasualServerWorker::CasualServerWorker(DownloadManager *downloadManager) 
    : downloadManager(downloadManager) {
//    printf("Created CasualServerWorker...\n");

    hasNoWork = true;

    if (pthread_create(&thread, NULL, serverWorkerRunThread, this)) {
        throw new ServerException("Creating thread");
        }
}

static void *serverWorkerRunThread(void *ptr) {
    ServerWorker *worker = static_cast<ServerWorker *>(ptr);
    worker->run();
    pthread_exit(NULL);
}

CasualServerWorker::CasualServerWorker(const CasualServerWorker& orig) {
}

CasualServerWorker::~CasualServerWorker() {
}

void CasualServerWorker::run() {
    //printf("CasualServerWorker::run()\n");

    fdmax = 0;

    while(1) {
        bool badConnection = false;

        while (!connectionManager.getConnectionCount())
            wait();

        connectionManager.updateWritingState();

        fd_set read_set = connectionManager.getReadSet();
        fd_set write_set = connectionManager.getWriteSet();

        timeval _timeval;
        _timeval.tv_sec = 1;
        _timeval.tv_usec = 0;

        if (select(fdmax+1, &read_set, &write_set, NULL, &_timeval) == -1) {
            //throw new ServerException("select() failed");
            //throw 1;
            Logger::instance().log("Select failed", ERROR);
            }

        for (int i = 0; i <= fdmax; i++) {
            try {
                if (FD_ISSET(i, &read_set)) {
                    connectionManager.recieve(i);
                    }
                else
                if (FD_ISSET(i, &write_set)) {
                    connectionManager.write(i);
                    }
                }

                catch (ClientClosedSocket &e) {
                    Logger::instance().log("Cliend closed socket (fd="+stringOf(i)+")", ERROR);
                    badConnection = true;
                    }

                catch (ServerException &e) {
                    //std::cout << "Exception: " << e.getMessage() << std::endl;
                    Logger::instance().log("Exception: "+e.getMessage(), ERROR);
                    badConnection = true;
                    }
                catch (HTTPParserException &e) {
                    Logger::instance().log("HTTPParser: "+e.getMessage(), ERROR);
                    badConnection = true;
                    }

                if (badConnection)  {
                    connectionManager.closeConnection(i); 
                    }

            }

        //std::cout << "Cycle ended..." << std::endl;

        }

    //printf("Finishing thread...\n");
}

void CasualServerWorker::addSocket(int fd) {
    //std::cout << "CasualServerWorker::addSocket()" << std::endl;
    fdmax = std::max(fdmax, fd);
    connectionManager.addExistingConnection(new ProxyClientConnection(fd, downloadManager));
    this->notify();
}

int CasualServerWorker::getConnectionCount() {
    return connectionManager.getConnectionCount();
}

void CasualServerWorker::addConnection(Connection *connection) {
    //std::cout << "CasualServerWorker::addConnection" << std::endl;
    fdmax = std::max(fdmax, connection->getFD());
    connectionManager.addExistingConnection(connection);
    this->notify();
}
