/* 
 * File:   ServerWorker.cpp
 * Author: ripper
 * 
 * Created on September 11, 2009, 12:31 PM
 */

#include "ServerWorker.h"

ServerWorker::ServerWorker()
{
}

ServerWorker::ServerWorker(const ServerWorker& orig) {
}

ServerWorker::~ServerWorker() {
}

void ServerWorker::wait() {
    Synchonizable::wait();
}

void ServerWorker::notify() {
    Synchonizable::notify();
}