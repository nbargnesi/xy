/*
 * Copyright 2011-2012 Nick Bargnesi <nick@den-4.com>. All rights reserved.
 *
 * This file is part of xy.
 *
 * XY is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XY is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with xy.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _XY_LOGGING_H_
#define _XY_LOGGING_H_ 1

#include "core.h"
#include <log4c.h>

#define INFO LOG4C_PRIORITY_INFO
#define WARN LOG4C_PRIORITY_WARN
#define FATAL LOG4C_PRIORITY_FATAL
#define DEBUG LOG4C_PRIORITY_DEBUG
#define TRACE LOG4C_PRIORITY_TRACE

bool logging_init();

log4c_category_t * get_logger(const char *);

void log_debug(const log4c_category_t *, const char *);

void log_info(const log4c_category_t *, const char *);

void log_fatal(const log4c_category_t *, const char *);

void log_warn(const log4c_category_t *, const char *);

void log_trace(const log4c_category_t *, const char *);

bool logging_terminate();

#endif

