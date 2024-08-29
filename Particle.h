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

#include "spark_wiring_flags.h"
#include "spark_wiring_json.h"
#include "spark_wiring_map.h"
#include "spark_wiring_string.h"
#include "spark_wiring_time.h"
#include "spark_wiring_variant.h"
#include "spark_wiring_vector.h"
#include "rng_hal.h"
#include "system_error.h"
#include "system_tick_hal.h"

using namespace spark;
// using namespace particle;
// using namespace std::literals::chrono_literals;

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

#if 0
// Now in logging.h
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

    void write(const char *data, size_t size) const {
        write(LOG_LEVEL_INFO, data, size);
    }
 
    void write(LogLevel level, const char *data, size_t size) const {
        char buf[512];
        if (size > (sizeof(buf) - 1)) {
            size = sizeof(buf) - 1;
        }
        strncpy(buf, data, size);
        buf[size] = 0;

        ::printf("%s", buf);
    }

    void dump(const void *data, size_t size) const {
        dump(LOG_LEVEL_TRACE, data, size);
    }

    void dump(LogLevel level, const void *data, size_t size) const {
        static const char hex[] = "0123456789abcdef";
        char buf[513]; // Hex data is flushed in chunks
        buf[sizeof(buf) - 1] = 0; // Compatibility callback expects null-terminated strings
        size_t offs = 0;
        for (size_t i = 0; i < size; ++i) {
            const uint8_t b = ((const uint8_t*)data)[i];
            buf[offs++] = hex[b >> 4];
            buf[offs++] = hex[b & 0x0f];
            if (offs == sizeof(buf) - 1) {
                printf("%s", buf);
                offs = 0;
            }
        }
        if (offs) {
            buf[offs] = 0;
            printf("%s", buf);
        }   
    }

    String name;
};
// spark_wiring_logging.h
#endif 

extern const Logger Log;

namespace particle { namespace protocol {
    const size_t MAX_OPTION_DELTA_LENGTH = 12;
    const size_t MAX_FUNCTION_KEY_LENGTH = 64;
    const size_t MAX_VARIABLE_KEY_LENGTH = 64;
    const size_t MAX_EVENT_NAME_LENGTH = 64;

    const size_t MAX_EVENT_DATA_LENGTH = 1024;
    const size_t MAX_FUNCTION_ARG_LENGTH = 1024;
    const size_t MAX_VARIABLE_VALUE_LENGTH = 1024;

}};

// system_cloud.h
const uint32_t PUBLISH_EVENT_FLAG_PUBLIC = 0x0;
const uint32_t PUBLISH_EVENT_FLAG_PRIVATE = 0x1;
const uint32_t PUBLISH_EVENT_FLAG_NO_ACK = 0x2;
const uint32_t PUBLISH_EVENT_FLAG_WITH_ACK = 0x8;

// spark_wiring_cloud.h
struct PublishFlagType; // Tag type for Particle.publish() flags
typedef particle::Flags<PublishFlagType, uint8_t> PublishFlags;
typedef PublishFlags::FlagType PublishFlag;

const PublishFlag PUBLIC(PUBLISH_EVENT_FLAG_PUBLIC);
const PublishFlag PRIVATE(PUBLISH_EVENT_FLAG_PRIVATE);
const PublishFlag NO_ACK(PUBLISH_EVENT_FLAG_NO_ACK);
const PublishFlag WITH_ACK(PUBLISH_EVENT_FLAG_WITH_ACK);



uint32_t millis();

using namespace spark;

#endif /* __PARTICLE_H */
