/* 
 * File:   RemoteFile.h
 * Author: ripper
 *
 * Created on September 25, 2009, 5:45 PM
 */

#ifndef _REMOTEFILE_H
#define	_REMOTEFILE_H

#include <list>
#include <vector>

typedef std::pair<char *, int> FilePart;

class RemoteFile {
public:
    RemoteFile();

    virtual ~RemoteFile();

    virtual bool isDownloaded() const = 0;
    virtual bool isOK() const = 0;
    virtual const FilePart &getReadyPart(int n) const = 0;
    virtual int getReadyPartsAmount() const = 0;
private:
    RemoteFile(const RemoteFile& orig);
};

#endif	/* _REMOTEFILE_H */

