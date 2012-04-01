#ifndef _FILEHEADERPARSER_H_
#define _FILEHEADERPARSER_H_

#include <string>

class FileHeaderParser {
	public:
		FileHeaderParser();
		virtual ~FileHeaderParser();

		void feed(char *data, int len);

		bool isDownloaded();

	private:
		void parse(std::string);
		bool checkEnd();

		std::string storedData;
		bool downloaded;
		bool headerPassed;
		bool lengthRecieved;

		int total;
		int contentLength;
};

#endif /* _FILEHEADERPARSER_H_ */ 
