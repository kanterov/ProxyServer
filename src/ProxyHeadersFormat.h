#ifndef _PROXY_HEADERS_FORMAT_H
#define _PROXY_HEADERS_FORMAT_H

#include <string>

#include "HTTPParser.h"

class ProxyHeadersFormat 
{
    public:
        static std::string getHeaders(const HTTPParser *parser);

};

#endif /* _PROXY_HEADERS_FORMAT_H */
