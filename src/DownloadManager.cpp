/* 
 * File:   DownloadManager.cpp
 * Author: ripper
 * 
 * Created on September 25, 2009, 5:43 PM
 */

#include <iostream>

#include "DownloadManager.h"
#include "MainServerWorker.h"
#include "NewConnectionAccepter.h"
#include "DownloadConnection.h"
#include "Logger.h"

DownloadManager::DownloadManager(NewConnectionAccepter *mainServerWorker)
    : mainServerWorker(mainServerWorker) {
}

DownloadManager::DownloadManager(const DownloadManager& orig) {
}

DownloadManager::~DownloadManager() {
}

const RemoteFile *DownloadManager::getFile(const HTTPParser *parser) {
    pthread_mutex_lock(&mutex);

    const std::string &absoluteUrl = parser->getAbsoluteUrl();
    const RemoteFile *remoteFile = NULL;


    if (fileCache.find(absoluteUrl) != fileCache.end()) { 
        remoteFile = fileCache[absoluteUrl];
        }
    else {
        DownloadConnection *connection = new DownloadConnection(parser);
        const RemoteFile *file = connection->getDownloadingFile();

        mainServerWorker->addNewConnection(connection);

        Logger::instance().log("Started downloading file: "+absoluteUrl, INFO);

        //TODO: FIXME: IF NO COOKIES
        fileCache[absoluteUrl] = file;

        if (file->isDownloaded()) { 
            }

        remoteFile = file;
    }
    
    pthread_mutex_unlock(&mutex);

    return remoteFile;
}

