/* 
 * File:   config.h
 * Author: ripper
 *
 * Created on September 17, 2009, 11:06 AM
 */

#ifndef _CONFIG_H
#define	_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

    #define DEFAULT_PORT 8888

//    #define OUTPUT_PACKETS

    const int THREAD_POOL_SIZE = 4;
    const int THREAD_WAIT_TIME = 1;
    const int LISTENER_BACKLOG = 10;
    const int CHUNK = 4096;


#ifdef	__cplusplus
}
#endif

#endif	/* _CONFIG_H */

