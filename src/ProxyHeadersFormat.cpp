#include "ProxyHeadersFormat.h"
#include "stringSplit.h"

#include <iostream>

std::string ProxyHeadersFormat::getHeaders(const HTTPParser *parser) {
    std::string ret;

    const std::string &url = parser->getUrl();
    const std::string &host = parser->getHost();
    const std::string httpVersion = parser->getHTTPVersion();
    int port = parser->getPort();

//    std::cout << "ProxyHeadersFormat::getHeaders port=" << port << std::endl;

    ret += "GET "+url+" "+httpVersion+"\r\n";

    ret += "User-Agent: Wget/1.11.4\r\n";
    ret += "Accept: */*\r\n";

    ret += "Host: "+host+":"+stringOf<int>(port);
    //if (port != 80)
    //    ret += ":"+port;
    ret += "\r\n";

    if (parser->gotCookies())
        ret += parser->getCookies()+"\r\n";

    ret += "\r\n";

    return ret;
}
