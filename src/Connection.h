/* 
 * File:   Connection.h
 * Author: ripper
 *
 * Created on September 30, 2009, 7:36 PM
 */

#ifndef _CONNECTION_H
#define	_CONNECTION_H

class Connection {
public:
    virtual ~Connection();

    virtual void read() = 0;
    virtual void write() = 0;
    virtual bool isTimeToRead() = 0;
    virtual bool isTimeToWrite() = 0;
    virtual bool isFinished() = 0;
    virtual int getFD() = 0;
};
#endif	/* _CONNECTION_H */

