#ifndef _STRING_UTILS_H_
#define _STRING_UTILS_H_

#include <string>
#include <iterator>
#include <set>
#include <algorithm>
#include <sstream>

template <class T>
T valueOfString(const std::string &str) {
    std::istringstream iss(str, std::istringstream::in);
    T obj;
    iss >> obj;
    
    return obj;
}

template <class T>
std::string stringOf(T object) {
    std::ostringstream os;
    os << object;
    return os.str();
}

#endif /* _STRING_UTILS_ */
