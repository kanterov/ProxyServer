/* 
 * File:   HTTPParser.cpp
 * Author: ripper
 * 
 * Created on September 24, 2009, 12:46 PM
 */

#include <iostream>
#include <sstream>
#include <list>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cstdlib>

#include "HTTPParser.h"
#include "stringSplit.h"
#include "Logger.h"

HTTPParserException::HTTPParserException(std::string message)
    : BasicException(message) {

}

HTTPParser::HTTPParser() {
    isParsed = false;
    hasGotHeader = false;
}

HTTPParser::~HTTPParser() {

}

HTTPParser::HTTPParser(const HTTPParser &orig) {
    throw 1;
}

void HTTPParser::feed(std::string data) {
    storedData += data;

    if (storedData.rfind("\r\n\r\n") != std::string::npos) {
        parse();
        } 
}

void HTTPParser::parse() {
    std::istringstream iss(storedData, std::istringstream::in);
    bool parsedGet = false;

    while(!iss.eof()) { 
        std::string line;
        std::getline(iss, line);
        line[line.length()-1] = 0;
        
        Logger::instance().log("HTTPParser: ["+line+"]", DEBUG);
        //std::cout << "Line: [" << line << "]" << std::endl;

        if (line.find("GET ") == 0) {
            parseGet(line);
            parsedGet = true;
            }
        else
        if (line.find("Cookies:") == 0) {
            parseCookies(line);
        }   
            
    }

    if (!parsedGet) {
        throw HTTPParserException("GET not found");
        }

    hasGotHeader = true;
}

void HTTPParser::parseGet(const std::string &line) { 
    std::vector<std::string> parts;
    split(line, parts, ' ', 3, false);

    absoluteUrl = url = parts[1];
    httpVersion = parts[2];

    if (httpVersion.find(char(0)) != -1)
        httpVersion.erase(httpVersion.find(char(0)));

    //std::cout << "Got GET: " << parts[1] << " " << parts[2] << std::endl;


    std::string lower_url;
    std::transform(url.begin(), url.end(), std::back_inserter(lower_url), tolower);
    std::string http_protocol = "http://";
    std::string hostname_end = "/";
    int http_begin = lower_url.find(http_protocol);
    if (http_begin >= 0) {
        http_begin += http_protocol.length();
        int http_end = lower_url.find(hostname_end, http_begin);
        std::string host_port;
        //host = std::string(url.begin()+http_end, url.end());
        if (http_end >= 0) {
            host_port = std::string(url, http_begin, http_end - http_begin);
            }
        else {
            host_port = std::string(url.begin()+http_begin, url.end());
            }

        int port_begin = host_port.find(":");

        if (port_begin >= 0) { 
            host = std::string(host_port.begin(), host_port.begin()+port_begin);
            port = valueOfString<int>(std::string(host_port.begin()+port_begin+1, host_port.end()));
            //std::cout << "port string: " << std::string(host_port.begin()+port_begin+1, host_port.end()) << std::endl;
            }
        else {
            host = host_port;
            port = 80;
            }

        url = std::string(url.begin()+http_end, url.end());
        //std::cout << "HOSTNAME: " << host << " PORT: " << port << "URL: " << url << std::endl;
        }
    else
        throw HTTPParserException("Error parsing GET: bad url format");

}

void HTTPParser::parseCookies(const std::string &line) {
    hasGotCookies = true;
    cookieString = line;
    //std::cout << "Got " << cookieString << std::endl;
}

bool HTTPParser::gotHeader() const {
    return hasGotHeader;
}

const std::string &HTTPParser::getHost() const {
    return host;
}

int HTTPParser::getPort() const {
    return port;
}

const std::string &HTTPParser::getUrl() const {
    return url;
}

bool HTTPParser::gotCookies() const {
    return hasGotCookies;
}

const std::string &HTTPParser::getCookies() const {
    return cookieString;
}

const std::string &HTTPParser::getHTTPVersion() const {
    return httpVersion;
}

const std::string &HTTPParser::getAbsoluteUrl() const {
    return absoluteUrl;
}
