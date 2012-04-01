/* 
 * File:   DownloadingFile.cpp
 * Author: ripper
 * 
 * Created on October 11, 2009, 10:50 PM
 */

#include <iostream>

#include "DownloadingFile.h"

DownloadingFile::DownloadingFile() {
    ok = true;
    downloaded = false;
    mutex = PTHREAD_MUTEX_INITIALIZER;
}

DownloadingFile::DownloadingFile(const DownloadingFile& orig) {
}

DownloadingFile::~DownloadingFile() {
}

bool DownloadingFile::isDownloaded() const {
    pthread_mutex_lock(&mutex);
    bool downloadedCopy = downloaded;
    pthread_mutex_unlock(&mutex);
    return downloaded;
}

bool DownloadingFile::isOK() const {
    pthread_mutex_lock(&mutex);
    bool okCopy = ok;
    pthread_mutex_unlock(&mutex);
    return ok;
}

void DownloadingFile::setIsDownloaded(bool val) {
    pthread_mutex_lock(&mutex);
    downloaded = val;
    pthread_mutex_unlock(&mutex);
}

void DownloadingFile::setIsOK(bool val) {
    pthread_mutex_lock(&mutex);
    ok = val;
    pthread_mutex_unlock(&mutex);
}

const FilePart &DownloadingFile::getReadyPart(int n) const {
    pthread_mutex_lock(&mutex);
    const FilePart &filePart = fileParts[n];
    pthread_mutex_unlock(&mutex);
    return filePart;
}

int DownloadingFile::getReadyPartsAmount() const {
    pthread_mutex_lock(&mutex);
    int size = fileParts.size();
    pthread_mutex_unlock(&mutex);

    return size;
}

void DownloadingFile::addPart(const FilePart &filePart) {
    if (filePart.second == 0)
        return;
    pthread_mutex_lock(&mutex);
    fileParts.push_back(filePart);
    pthread_mutex_unlock(&mutex);
}

