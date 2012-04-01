/* 
 * File:   MainServerWorker.cpp
 * Author: ripper
 * 
 * Created on September 11, 2009, 12:35 PM
 */

#include <sys/socket.h>  /* for socket(), bind(), and connect() */
#include <sys/select.h>  /* for fd_set */
#include <arpa/inet.h>   /* for sockaddr_in and inet_ntoa() */
#include <memory.h>      /* for memset */
#include <limits>        /* for numeric_limits */
#include <errno.h>

#include "MainServerWorker.h"
#include "ServerException.h"
#include "CasualServerWorker.h"
#include "ProxyClientConnection.h"
#include "config.h"
#include "Logger.h"
#include "StringUtils.h"

MainServerWorker::MainServerWorker(int port = DEFAULT_PORT)
    : port(port), downloadManager(this)
{
    int yes = 1;

    if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        throw ServerException("socket() failed"); // caught
        }

    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        throw ServerException("setsockopt() failed"); // caught
        }

    if ((server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        throw ServerException("socket() failed"); // caught

    workers.push_back(this);
    for (int i = 0; i < THREAD_POOL_SIZE - 1; i++) {
        workers.push_back(new CasualServerWorker(&downloadManager));
    }
}

MainServerWorker::MainServerWorker(const MainServerWorker& orig) 
    : downloadManager(this) {
}

MainServerWorker::~MainServerWorker() {
}

void MainServerWorker::doBind() {
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(port);

    memset(&(serveraddr.sin_zero), '\0', 8);

    if (bind(listener, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1) {
        throw ServerException("bind() failed"); // caught
        }
}

void MainServerWorker::addSocket(int fd) {
    Logger::instance().log("MainServerWorker::addSocket(fd="+stringOf(fd)+")", DEBUG);
    fdmax = std::max(fdmax, fd);
    connectionManager.addExistingConnection(new ProxyClientConnection(fd, &downloadManager));
}

int MainServerWorker::getConnectionCount() {
    return connectionManager.getConnectionCount();
}

ServerWorker *MainServerWorker::getOptimalWorker() {
    ServerWorker *optimalWorker = NULL;
    int min = std::numeric_limits<int>::max();

    for (std::list<ServerWorker *>::iterator i = workers.begin(); i != workers.end(); i++) {
        if ((*i)->getConnectionCount() < min) {
            optimalWorker = (*i);
            min = (*i)->getConnectionCount();
            }
        }
    
    return optimalWorker;    
}


void MainServerWorker::addNewConnection(Connection *connection) {
    Logger::instance().log("MainServerWorker::addNewConnection(connection.fd="+stringOf(connection->getFD())+")", DEBUG);
    ServerWorker *optimalWorker = getOptimalWorker();
    optimalWorker->addConnection(connection);
}

void MainServerWorker::addConnection(Connection *connection) {
    Logger::instance().log("MainServerWorker::addConnection(connection.fd="+stringOf(connection->getFD())+")", DEBUG);
    fdmax = std::max(fdmax, connection->getFD());
    connectionManager.addExistingConnection(connection);
}

void MainServerWorker::doAccept() {
    struct sockaddr_in clientaddr;
    int newfd;
    int addrlen = sizeof(clientaddr);

    if ((newfd = accept(listener, (struct sockaddr *)&clientaddr, (socklen_t *)&addrlen)) == -1) {
        Logger::instance().log("Error accepting connection", ERROR);
        }
    else {
        if (newfd > fdmax) {
            fdmax = newfd;
        }

        ServerWorker *optimalWorker = getOptimalWorker();
        optimalWorker->addSocket(newfd);

        Logger::instance().log("New connection from "+
            std::string(inet_ntoa(clientaddr.sin_addr))+
            " on socket "+stringOf<int>(newfd), INFO);
        }

}

void MainServerWorker::run() {
    int oldConnectionCount = 0;
    //printf("MainThread::run()\n");

    doBind();

    if (listen(listener, LISTENER_BACKLOG) == -1) {
        throw ServerException("listen() failed"); // caught
        }

    connectionManager.addListener(listener);
    fdmax = listener;

    while(1) {
        bool badConnection = false;

        if (oldConnectionCount != connectionManager.getConnectionCount()) {
            //std::cout << "Connection count changed from " << oldConnectionCount << " to " << connectionManager.getConnectionCount() << std::endl;
            Logger::instance().log("Connection count changed from "+stringOf(oldConnectionCount)+
                " to "+stringOf(connectionManager.getConnectionCount()), DEBUG);
            oldConnectionCount = connectionManager.getConnectionCount();
            }

        timeval _timeval;
        _timeval.tv_sec = 1;
        _timeval.tv_usec = 0;

        connectionManager.updateWritingState();

        fd_set read_set = connectionManager.getReadSet();
        fd_set write_set = connectionManager.getWriteSet();

               

        int err;
        if ((err = select(fdmax+1, &read_set, &write_set, NULL, &_timeval)) == -1) {
            //throw new ServerException("select() failed");
            Logger::instance().log("Select failed", ERROR);
            continue;
            }

            for (int i = 0; i <= fdmax; i++) {
                try {
                    if (FD_ISSET(i, &read_set)) {
                        if(i == listener) {
                            /* new connection */
                            doAccept();
                            }
                        else {
                            connectionManager.recieve(i);
                            }
                        }
                    else
                    if (FD_ISSET(i, &write_set)) {
                        if (i == listener) continue;

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
        }
}

void MainServerWorker::wait() { }
void MainServerWorker::notify() { }
