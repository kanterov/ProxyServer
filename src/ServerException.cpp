#include "ServerException.h"

ServerException::ServerException(std::string message)
    : BasicException(message) {
}

ServerReadException::ServerReadException()
    : ServerException("Socket read error") {

}

ClientClosedSocket::ClientClosedSocket()
    : ServerException("Client closed socket") {

}
