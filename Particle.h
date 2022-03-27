// Dummy particle.h file for testing logdata.cpp module from gcc
#ifndef __PARTICLE_H
#define __PARTICLE_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <cassert>

#include "spark_wiring_json.h"
#include "spark_wiring_string.h"
#include "spark_wiring_time.h"
#include "rng_hal.h"
#include "system_tick_hal.h"

class Stream {
public:
	inline int available() { return 0; }
	inline int read() { return 0; }
};


// Doesn't actually work as we don't support threads in the gcclib, but makes it easier to compile code
class Mutex
{
public:
    /**
     * Creates a new mutex.
     */
    Mutex() {};

    ~Mutex() {
    }

    void lock() {  }
    bool trylock() { return true; }
    bool try_lock() { return true; }
    void unlock() { }

};

#define SINGLE_THREADED_SECTION()
#define SINGLE_THREADED_BLOCK()
#define WITH_LOCK(x)
#define TRY_LOCK(x)


typedef enum LogLevel {
    LOG_LEVEL_ALL = 1, // Log all messages
    LOG_LEVEL_TRACE = 1,
    LOG_LEVEL_INFO = 30,
    LOG_LEVEL_WARN = 40,
    LOG_LEVEL_ERROR = 50,
    LOG_LEVEL_PANIC = 60,
    LOG_LEVEL_NONE = 70 // Do not log any messages
} LogLevel;

class Logger {
public:
	Logger(const char *name) : name(name) {};

    void trace(const char *fmt, ...) const __attribute__((format(printf, 2, 3))) { // First argument is implicit 'this'
        va_list ap;
        va_start(ap, fmt);
        vprintf(LOG_LEVEL_TRACE, fmt, ap);
        va_end(ap);
    }
    /*!
        \brief Generates info message.
        \param fmt Format string.
    */
    void info(const char *fmt, ...) const __attribute__((format(printf, 2, 3))) {
        va_list ap;
        va_start(ap, fmt);
        vprintf(LOG_LEVEL_INFO, fmt, ap);
        va_end(ap);
    }
    /*!
        \brief Generates warning message.
        \param fmt Format string.
    */
    void warn(const char *fmt, ...) const __attribute__((format(printf, 2, 3))) {
        va_list ap;
        va_start(ap, fmt);
        vprintf(LOG_LEVEL_WARN, fmt, ap);
        va_end(ap);
    }
    /*!
        \brief Generates error message.
        \param fmt Format string.
    */
    void error(const char *fmt, ...) const __attribute__((format(printf, 2, 3))) {
        va_list ap;
        va_start(ap, fmt);
        vprintf(LOG_LEVEL_ERROR, fmt, ap);
        va_end(ap);
    }

    void log(LogLevel level, const char *fmt, ...) const __attribute__((format(printf, 3, 4))) {
        va_list ap;
        va_start(ap, fmt);
        vprintf(level, fmt, ap);
        va_end(ap);
    }
    
    void vprintf(LogLevel level, const char *fmt, va_list ap) const {
        char buf[512];
        vsnprintf(buf, sizeof(buf), fmt, ap);
        const char *levelStr;
        switch(level) {
            case LOG_LEVEL_TRACE:
                levelStr = "TRACE";
                break;
            case LOG_LEVEL_INFO:
                levelStr = "INFO";
                break;
            case LOG_LEVEL_WARN:
                levelStr = "WARN";
                break;
            case LOG_LEVEL_ERROR:
                levelStr = "ERROR";
                break;
            case LOG_LEVEL_PANIC:
                levelStr = "PANIC";
                break; 
            default:
                levelStr = "UNKNOWN";
                break;               
        }

        ::printf("%s %s: %s\n", name.c_str(), levelStr, buf);
    }

    String name;
};

namespace particle { namespace protocol {
    const size_t MAX_OPTION_DELTA_LENGTH = 12;
    const size_t MAX_FUNCTION_KEY_LENGTH = 64;
    const size_t MAX_VARIABLE_KEY_LENGTH = 64;
    const size_t MAX_EVENT_NAME_LENGTH = 64;

    const size_t MAX_EVENT_DATA_LENGTH = 1024;
    const size_t MAX_FUNCTION_ARG_LENGTH = 1024;
    const size_t MAX_VARIABLE_VALUE_LENGTH = 1024;

}};


uint32_t millis();

using namespace spark;

#endif /* __PARTICLE_H */
