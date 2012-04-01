#include <vector>

#include "FileHeaderParser.h"
#include "StringUtils.h"
#include "stringSplit.h"
#include "Logger.h"

FileHeaderParser::FileHeaderParser() {
	headerPassed = false;
	downloaded = false;
	lengthRecieved = false;

	total = 0;
}


FileHeaderParser::~FileHeaderParser() {


}


bool FileHeaderParser::checkEnd() {
	if (lengthRecieved) {
		if (total >= contentLength) {
			downloaded = true;
			return true;
			}
		}
	return false;
}

void FileHeaderParser::feed(char *data, int len) {
	if (headerPassed) {
		total += len;
		checkEnd();
		}
	else
		parse(std::string(data, data+len));
}

void FileHeaderParser::parse(std::string data) {
	storedData += data;

	if (storedData.find("\r\n\r\n") != 0) {
		headerPassed = true;
		total += storedData.length() - storedData.find("\r\n\r\n") - 4;

		std::istringstream iss(storedData, std::istringstream::in);

		while(!iss.eof()) { 
			std::string line;
			std::getline(iss, line);
			if (line == "\r")
				break;

			line[line.length()-1] = 0;
		

			Logger::instance().log("FileHeaderParser: ["+line+"]", DEBUG);
			//std::cout << "Line: [" << line << "]" << std::endl;

			if (line.find("Content-Length:") == 0) {
				std::vector<std::string> parts;
				split(line, parts, ':', 2); 
				lengthRecieved = true;

				contentLength = valueOfString<int>(parts[1]);
				Logger::instance().log("GOT Content-Length: "+stringOf(contentLength), DEBUG);

				}
			}

			checkEnd();
		}
}

bool FileHeaderParser::isDownloaded() {
	return downloaded;
}
