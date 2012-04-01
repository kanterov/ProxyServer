/* 
 * File:   DownloadingFile.h
 * Author: ripper
 *
 * Created on October 11, 2009, 10:50 PM
 */

#ifndef _DOWNLOADINGFILE_H
#define	_DOWNLOADINGFILE_H

#include <vector>

#include "RemoteFile.h"

class DownloadingFile : public RemoteFile {
public:
    DownloadingFile();
    virtual ~DownloadingFile();


    bool isDownloaded() const;
    bool isOK() const;
    const FilePart &getReadyPart(int n) const;
    int getReadyPartsAmount() const;

    void setIsDownloaded(bool val);
    void setIsOK(bool val);
    void addPart(const FilePart &filePart);
private:
    DownloadingFile(const DownloadingFile& orig);

    bool downloaded;
    bool ok;

    std::vector<FilePart> fileParts;

    mutable pthread_mutex_t mutex;
};

#endif	/* _DOWNLOADINGFILE_H */

