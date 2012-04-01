#ifndef __SERVER_EXCEPTION_H__
#define __SERVER_EXCEPTION_H__

#include "BasicException.h"

class ServerException : public BasicException {
    public:
        ServerException(std::string message);
};

class ServerReadException : public ServerException{
    public:
        ServerReadException();
};

class ClientClosedSocket : public ServerException{
    public:
        ClientClosedSocket();
};


#endif /* __SERVER_EXCEPTION_H__ */
