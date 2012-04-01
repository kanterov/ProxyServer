/* 
 * File:   Connection.cpp
 * Author: ripper
 * 
 * Created on September 11, 2009, 12:33 PM
 */

#include <iostream>
#include <sys/socket.h>

#include "ProxyClientConnection.h"
#include "config.h"
#include "ServerException.h"
#include "DownloadConnection.h"
#include "Logger.h"
#include "StringUtils.h"

ProxyClientConnection::ProxyClientConnection(int fd, DownloadManager *downloadManager)
    : fd(fd), downloadManager(downloadManager)
{
    Logger::instance().log("ProxyClientConnection created (fd="+stringOf(fd)+")", DEBUG);
    timeToRead = true;
    timeToWrite = false;
    finished = false;
    partsSend = 0;
    sendingPartProgress = 0;
    //file = NULL;
}

ProxyClientConnection::ProxyClientConnection(const ProxyClientConnection& orig) {
}

ProxyClientConnection::~ProxyClientConnection() {
}

void ProxyClientConnection::read() {
//    std::cout << "Connection#" << fd << "@" << (int) pthread_self() << ": read()" << std::endl;
    Logger::instance().log("ProxyClientConnection read (fd="+stringOf(fd)+")", DEBUG);

    char buf[CHUNK];
    int n;

    if ((n = recv(fd, buf, sizeof(buf), 0)) <= 0) {
        /* got error or connection closed by client */
        if (n == 0) {
            /* connection closed */
            throw ClientClosedSocket();
            }
        else {
            close(fd);
            throw ServerReadException();
            }

        }
    else { /* got some data */
        //std::cout << "Connection#" << fd << "@" << (int) pthread_self() << ": " << buf << std::endl;
        httpParser.feed(std::string(buf, n));

        if (httpParser.gotHeader()) {
//            std::cout << "ProxyClientConnection::read::gotHeader" << std::endl;
            absoluteUrl = httpParser.getAbsoluteUrl(); // stupid for logging
            timeToRead = false;
            timeToWrite = true;
            file = downloadManager->getFile(&httpParser);
        }
   }
}

void ProxyClientConnection::write() {
    Logger::instance().log("ProxyClientConnection write (fd="+stringOf(fd)+")", DEBUG);

    if (file == NULL) return;
    //const RemoteFile *file = downloadManager->getFile(&httpParser);
    int readyParts = file->getReadyPartsAmount();
    //std::cout << "ProxyClientConnection::write, readyParts=" << readyParts \
    //    << " partsSend=" << partsSend << " sendingPartProgress=" << sendingPartProgress << std::endl;
    
    if (readyParts > partsSend) {
        int i;

        for (i = partsSend; i < readyParts; i++) {
            const FilePart &part = file->getReadyPart(i);
            const int size = part.second;
            const int sizeToSend = size-sendingPartProgress;

            int n = send(fd, part.first+sendingPartProgress, \
                    sizeToSend, NULL);
       
             if (n == 0) {
                /* connection closed */
                throw ClientClosedSocket();
                }
            else  
            if (n < 0) {
                close(fd);
                throw new ServerException("ProxyClientConnection::write");
                }
            else
            if (n == sizeToSend) {
                sendingPartProgress = 0;
                }
            else {
                sendingPartProgress += n;
                break;
                }
        
            sendingPartProgress = 0;
            }

        partsSend = i;
        }
    
    if (file->isDownloaded() && readyParts == partsSend) {
        Logger::instance().log("File downloaded: "+absoluteUrl, INFO);
        //std::cout << "File downloaded by client!" << std::endl;
        //TODO: Interrupt, close connection
        finished = true;
        close(fd);
        }

    //std::cout << "ProxyClientConnection::write -- exit" << std::endl;
}

bool ProxyClientConnection::isTimeToRead() {
    return timeToRead;
}

bool ProxyClientConnection::isTimeToWrite() {
	if (file && timeToWrite) {
		int readyParts = file->getReadyPartsAmount();
//		std::cout << "ReadyParts: " << readyParts << " PartsToSend: " << partsSend << " isDownloaded: " << file->isDownloaded() << " url: " << absoluteUrl << std::endl;
		if (readyParts > partsSend)
			return true;
		else
			return false;
		}

    return timeToWrite;
}

int ProxyClientConnection::getFD() {
    return fd;
}

bool ProxyClientConnection::isFinished() {
    return finished;
}
