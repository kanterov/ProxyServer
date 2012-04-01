#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <boost/noncopyable.hpp>
#include <string>

enum LoggerPriority { FATALERROR, ERROR, INFO, DEBUG };

class Logger : private boost::noncopyable {
    public:

        static Logger& instance();
        static void destroy();
        void log(const std::string &message, LoggerPriority priority);
        
    private:
        Logger();
        ~Logger();

        static void initialize()
        {
            _instance = new Logger();
        }

        static Logger* _instance;
        static pthread_once_t is_initialised;

        static pthread_mutex_t loggerMutex;
};

#endif /* _LOGGER_H_ */
