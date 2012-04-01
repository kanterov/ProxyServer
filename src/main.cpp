/* 
 * File:   main.cpp
 * Author: ripper
 *
 * Created on September 11, 2009, 12:30 PM
 */

#include <iostream>

#include "ServerWorker.h"
#include "MainServerWorker.h"

#include "ServerException.h"
#include "Logger.h"

/*
 * 
 */
int main(int argc, char** argv) {
    ServerWorker *server = new MainServerWorker(8888);

    try {
        server->run();
        }

    catch(ServerException &a) {
        Logger::instance().log(a.getMessage(), FATALERROR);
        }
    delete server;
    pthread_exit(NULL);
}

