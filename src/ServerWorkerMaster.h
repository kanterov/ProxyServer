/* 
 * File:   ServerWorkerMaster.h
 * Author: ripper
 *
 * Created on September 17, 2009, 1:06 PM
 */

#ifndef _SERVERWORKERMASTER_H
#define	_SERVERWORKERMASTER_H

class ServerWorkerMaster {
public:
    ServerWorkerMaster();
    virtual ~ServerWorkerMaster();

    void threadStarted();
private:
    ServerWorkerMaster(const ServerWorkerMaster& orig);

};

#endif	/* _SERVERWORKERMASTER_H */

