#include "DownloadConnection.h"
#include "ServerException.h"
#include "ProxyHeadersFormat.h"
#include "config.h"
#include "ServerException.h"
#include "Logger.h"


DownloadConnection::DownloadConnection(const HTTPParser *parser) {
    int port;
    const char *host;

    if (!parser->gotHeader())
        throw new ServerException("DownloadConnection::DownloadConnection()"
                            " --- Creation from unfinished header");

    port = parser->getPort();
    host = parser->getHost().c_str();

    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if(fd == -1) {
        //std::cout << "DownloadConnection::DownloadConnection :: Could not make socket" << std::endl;
        throw new ServerException("Could not make socket");
        }

    hostInfo = gethostbyname(host);

    memcpy(&hostAddr, hostInfo->h_addr, hostInfo->h_length);

    address.sin_addr.s_addr = hostAddr;
    address.sin_port = htons(port);
    address.sin_family = AF_INET;

    if (connect(fd, (struct sockaddr*)&address, sizeof(address)) == -1)  {
        //std::cout << "DownloadConnection::DownloadConnection :: Could not connect to host" << std::endl;
        throw new ServerException("Could not connect to host");
        }

    readyToRead = false;
    readyToWrite = true;
    finished = false;

    headersToSend = ProxyHeadersFormat::getHeaders(parser);
    //std::cout << "HeadersToSend: " << headersToSend << std::endl;
    
    dataLeftToSend = headersToSend.length();
    writePosition = 0;
}

DownloadConnection::~DownloadConnection() {
}

void DownloadConnection::read() {
    char buf[CHUNK];
    int n;

    if ((n = recv(fd, buf, sizeof(buf), 0)) <= 0) {
        /* got error or connection closed by client */

        if (n == 0) {
            /* connection closed */
            //throw new ClientClosedSocket();
            //std::cout << "Downloading file finished!" << std::endl;
            downloadingFile.setIsDownloaded(true);
            }
        else {
            //throw new ServerReadException();
            downloadingFile.setIsOK(false);
            }

        finished = true;
        close(fd);
        }
    else { /* got some data */
#ifdef OUTPUT_PACKETS        
        std::cout << "DownloadConnection: " << std::string(buf,buf+n) << std::endl;
#endif        
        char *buffered_data = new char [n];
        memcpy(buffered_data, buf, n);
		fileHeaderParser.feed(buffered_data, n);
		if (fileHeaderParser.isDownloaded()) {
			Logger::instance().log("Recieved file by full Content-Length", INFO);
			finished = true;
			close(fd);
			}

        downloadingFile.addPart(std::pair<char *, int>(buffered_data, n));

		if (fileHeaderParser.isDownloaded()) {
			downloadingFile.setIsDownloaded(true);
			}

        }
}

void DownloadConnection::write() {
    int count = send(fd, headersToSend.c_str()+writePosition,\
            dataLeftToSend, NULL);

    if (count < 0)
        throw new ServerException("DownloadConnection::write");
    else {
#ifdef OUTPUT_PACKETS
        std::cout << "DownloadConnection: " << std::string(headersToSend.begin()+writePosition, headersToSend.begin()+count+writePosition) << std::endl;
#endif
        writePosition += count;
        dataLeftToSend -= count;
        }

    if (dataLeftToSend == 0) {
        readyToWrite = false;
        readyToRead = true;
        }
}

bool DownloadConnection::isTimeToRead() {
    return readyToRead;
}

bool DownloadConnection::isTimeToWrite() {
    return readyToWrite;
}

int DownloadConnection::getFD() {
    return fd;
}

const RemoteFile *DownloadConnection::getDownloadingFile() {
    return &downloadingFile;
}

bool DownloadConnection::isFinished() {
    return finished;
}
