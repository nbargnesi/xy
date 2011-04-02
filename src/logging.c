#include "logging.h"

bool logging_init() {
    if (log4c_init() == 0)
        return true;
    return false;
}

log4c_category_t * get_logger(const char *cat) {
    return log4c_category_get(cat);
}

static void __log__(const log4c_category_t *cat, int pri, const char *msg) {
    log4c_category_log(cat, pri, msg);
}

void log_debug(const log4c_category_t *cat, const char *msg) {
    __log__(cat, DEBUG, msg);
}

void log_info(const log4c_category_t *cat, const char *msg) {
    __log__(cat, INFO, msg);
}

void log_fatal(const log4c_category_t *cat, const char *msg) {
    __log__(cat, FATAL, msg);
}

void log_warn(const log4c_category_t *cat, const char *msg) {
    __log__(cat, WARN, msg);
}

void log_trace(const log4c_category_t *cat, const char *msg) {
    __log__(cat, TRACE, msg);
}

bool logging_terminate() {
    if (log4c_fini() == 0)
        return true;
    return false;
}

