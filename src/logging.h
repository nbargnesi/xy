#ifndef LOGGING_H_
#define LOGGING_H_

#include "core.h"
#include <log4c.h>

#define INFO LOG4C_PRIORITY_INFO
#define WARN LOG4C_PRIORITY_WARN
#define FATAL LOG4C_PRIORITY_FATAL
#define DEBUG LOG4C_PRIORITY_DEBUG
#define TRACE LOG4C_PRIORITY_TRACE

bool start_logging();

log4c_category_t * get_logger(const char *);

void log_debug(const log4c_category_t *, const char *);

void log_info(const log4c_category_t *, const char *);

void log_fatal(const log4c_category_t *, const char *);

void log_warn(const log4c_category_t *, const char *);

void log_trace(const log4c_category_t *, const char *);

bool stop_logging();

#endif

