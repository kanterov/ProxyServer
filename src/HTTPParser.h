/* 
* File:   HTTPParser.h
 * Author: ripper
 *
 * Created on September 24, 2009, 12:46 PM
 */

#ifndef _HTTPPARSER_H
#define	_HTTPPARSER_H

#include <string>

#include "BasicException.h"

class HTTPParserException : public BasicException {
    public:
        HTTPParserException(std::string message);

};

class HTTPParser {
    public:
        HTTPParser();
        virtual ~HTTPParser();

        void feed(std::string data);
        bool gotHeader() const;
        bool gotCookies() const;

        const std::string &getHost() const;
        const std::string &getUrl() const;
        const std::string &getCookies() const;
        const std::string &getHTTPVersion() const;
        const std::string &getAbsoluteUrl() const;
        int getPort() const;
    private:
        HTTPParser(const HTTPParser& orig);
        void parse();
        void parseGet(const std::string &line);
        void parseCookies(const std::string &line);

        bool isParsed;
        bool hasGotHeader;
        bool hasGotCookies;

        std::string storedData;
        std::string url;
        std::string httpVersion;
        std::string cookieString;
        std::string host;
        std::string absoluteUrl;
        int port;
};

#endif	/* _HTTPPARSER_H */

