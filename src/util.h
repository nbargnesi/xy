/*
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

#ifndef UTIL_H_
#define UTIL_H_

#include "core.h"

/*
 * Function: dump_stack
 *
 * Dumps some number of stack frames to stderr.
 */
void dump_stack(int);

/*
 * Function: parse_command
 */
void parse_command(char *, char **);

/*
 * Function: exec
 *
 * Executes the supplied command, forking first.
 */
void exec(const char *);

/*
 * Function: change_name
 *
 * TODO Document me.
 */
void change_name(Display *, const char *);

/*
 * Function: streq
 *
 * TODO Document me.
 */
bool streq(const char *, const char *);

/*
 * Function: trim
 *
 * TODO Document me.
 */
char * trim(char *str);

void restart(const char *);

#endif

