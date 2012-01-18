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

#ifndef _XY_UTIL_H_
#define _XY_UTIL_H_ 1

#include "core.h"

#define MAX_PROC_NAME_LEN 16

void dump_stack(int);
void parse_command(char *, char **);
pid_t exec(const char *);
char * rc_path();

/*
 * Function: change_name
 *
 * Changes the window manager's name.
 *
 * Parameters:
 *
 *  Display * - The display to use.
 *  const char * - The name to use; NULL will return immediately
 */
void change_name(Display *, const char *);

bool streq(const char *, const char *);
char * trim(char *str);

/**
 * Function: restart
 *
 * Invokes execlp using using the supplied command.
 *
 * Parameters:
 *  
 *  char * - command to pass to execlp
 */
void restart(const char *);

void set_process_name(const char *);

#endif

