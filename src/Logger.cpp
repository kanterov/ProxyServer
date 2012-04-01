#include "Logger.h"
#include "config.h"

#include <iostream>
#include <string.h>

Logger* Logger::_instance;
pthread_once_t Logger::is_initialised = PTHREAD_ONCE_INIT;
pthread_mutex_t Logger::loggerMutex = PTHREAD_MUTEX_INITIALIZER;

Logger::Logger() {
}

Logger::~Logger() {
   pthread_mutex_destroy(&loggerMutex); 
}

Logger& Logger::instance() {
    pthread_once(&is_initialised, &initialize);
    return *_instance;
}

void Logger::destroy() {
    pthread_mutex_lock(&loggerMutex);
    delete _instance;
    _instance = NULL;
    is_initialised = PTHREAD_ONCE_INIT;
    loggerMutex = PTHREAD_MUTEX_INITIALIZER;
}

void Logger::log(const std::string &message, LoggerPriority priority) {
    pthread_mutex_lock(&loggerMutex);
    time_t timeNow = time(NULL);
    char *timeString = ctime(&timeNow);
    timeString[strlen(timeString) - 1] = 0; // remove \n
    
    std::cout << timeString << ": " << message << std::endl;
    pthread_mutex_unlock(&loggerMutex);
}

