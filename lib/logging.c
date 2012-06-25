/*
 * Copyright (c) 2011-2012 Nick Bargnesi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

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

