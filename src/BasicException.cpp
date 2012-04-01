#include "BasicException.h"

BasicException::BasicException(const std::string &message) 
    : message(message) {
    
}

const std::string &BasicException::getMessage() const {
    return message;
}
