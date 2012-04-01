#ifndef _DOWNLOAD_CONNECTION_H
#define _DOWNLOAD_CONNECTION_H

#include <sys/socket.h>
#include <netdb.h>
#include <memory.h>
#include <iostream>

#include "HTTPParser.h"
#include "Connection.h"
#include "DownloadingFile.h"
#include "FileHeaderParser.h"

class DownloadConnection : public Connection {
    public:
        DownloadConnection(const HTTPParser *parser);
        virtual ~DownloadConnection();

        virtual void read();
        virtual void write();
        
        virtual bool isTimeToRead();
        virtual bool isTimeToWrite();
        virtual bool isFinished();

        virtual int getFD();
        virtual const RemoteFile *getDownloadingFile();


    private:
        DownloadConnection(const DownloadConnection &orig);

        int writePosition;
        int dataLeftToSend;
        int fd;
        struct hostent* hostInfo;
        int hostAddr;
        struct sockaddr_in address;

        bool readyToRead;
        bool readyToWrite;
        bool finished;

        std::string headersToSend;
        
        DownloadingFile downloadingFile;
		FileHeaderParser fileHeaderParser;
};

#endif /* _DOWNLOAD_CONNECTION_H */ 
