/* 
 * File:   NewConnectionAccepter.h
 * Author: ripper
 *
 * Created on October 11, 2009, 11:28 PM
 */

#ifndef _NEWCONNECTIONACCEPTER_H
#define	_NEWCONNECTIONACCEPTER_H

#include "Connection.h"

class NewConnectionAccepter {
public:
    NewConnectionAccepter();
    virtual ~NewConnectionAccepter();

    virtual void addNewConnection(Connection *connection) = 0;
private:
    NewConnectionAccepter(const NewConnectionAccepter& orig);

};

#endif	/* _NEWCONNECTIONACCEPTER_H */

