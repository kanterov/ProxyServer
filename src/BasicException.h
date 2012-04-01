#ifndef __BASIC_EXCEPTION_H__
#define __BASIC_EXCEPTION_H__

#include <string>

class BasicException {
    public:
        BasicException(const std::string &message);

        virtual const std::string &getMessage() const;
    private:
        std::string message;
};

#endif /* __BASIC_EXCEPTION_H__ */
