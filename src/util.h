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
 *
 * Parameters:
 *
 *  int - number of frames
 */
void dump_stack(int);

/*
 * Function: parse_command
 *
 * Parses the string into pointers to strings, tokenized by " ".
 *
 * Parameters:
 *
 *  char * - the string to parse
 *  char ** - the returned strings
 */
void parse_command(char *, char **);

/*
 * Function: exec
 *
 * Executes the supplied command by calling *execvp*.  The command will be
 * parsed before the exec call.
 *
 * Parameters:
 *
 *  const char * - command to execute
 *
 * See <parse_command>.
 */
void exec(const char *);

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

/*
 * Function: streq
 * 
 * Parameters:
 *  const char * - string one
 *  const char * - string two
 *
 * Returns true if:
 * (code)
 * str1 == NULL && str2 == NULL
 * (end)
 * or
 * (code)
 * strcmp(str1, str2) == 0
 * (end)
 */
bool streq(const char *, const char *);

/*
 * Function: trim
 *
 * Remove the leading and trailing whitespace from the string, returning
 * a pointer to the trimmed string.
 *
 * Parameters:
 *
 *  char * - String to trim
 */
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

#endif

