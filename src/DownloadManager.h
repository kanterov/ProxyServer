/* 
 * File:   DownloadManager.h
 * Author: ripper
 *
 * Created on September 25, 2009, 5:43 PM
 */

#ifndef _DOWNLOADMANAGER_H
#define	_DOWNLOADMANAGER_H

#include <string>
#include <map>

#include "RemoteFile.h"
#include "HTTPParser.h"
#include "NewConnectionAccepter.h"

class DownloadManager {
public:
    DownloadManager(NewConnectionAccepter *mainServerWorker);

    const RemoteFile *getFile(const HTTPParser *parser);

    virtual ~DownloadManager();
private:
    DownloadManager(const DownloadManager& orig);

    NewConnectionAccepter *mainServerWorker;
    std::map<std::string, const RemoteFile *> fileCache;
    mutable pthread_mutex_t mutex;

};

#endif	/* _DOWNLOADMANAGER_H */

