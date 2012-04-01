/* 
 * File:   Connection.h
 * Author: ripper
 *
 * Created on September 11, 2009, 12:33 PM
 */

#ifndef _PROXY_CLIENT_CONNECTION_H
#define	_PROXY_CLIENT_CONNECTION_H

#include "HTTPParser.h"
#include "Connection.h"
#include "MainServerWorker.h"
#include "DownloadManager.h"

class ProxyClientConnection : public Connection {
public:
    ProxyClientConnection(int fd, DownloadManager *downloadManager);
    
    virtual ~ProxyClientConnection();

    virtual void read();
    virtual void write();
    virtual bool isTimeToRead();
    virtual bool isTimeToWrite();
    virtual bool isFinished();

    virtual int getFD();
private:
    ProxyClientConnection(const ProxyClientConnection& orig);

    bool timeToRead;
    bool timeToWrite;
    bool finished;

    HTTPParser httpParser;
    DownloadManager *downloadManager;
    const RemoteFile *file;

    std::string absoluteUrl;

    int fd;
    int partsSend;
    int sendingPartProgress;
};

#endif	/* _PROXY_CLIENT_CONNECTION_H */

