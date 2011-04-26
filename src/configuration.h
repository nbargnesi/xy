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

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include "types.h"

struct ConfigEntry {
    char *name;
    char *value;
    struct ConfigEntry *next;
};

typedef struct ConfigEntry CONFIG_ENTRY;

struct Config {
    struct ConfigEntry *head;
    struct ConfigEntry *tail;
    uint num_entries;
};

typedef struct Config CONFIG;

CONFIG * get_config(const char *);

CONFIG * empty_config();

void free_config(CONFIG *);

const char * get_config_value(CONFIG *, const char *);

/*
 * Function: configure
 *
 * Processes the application's configuration.
 */
void configure(CONFIG *);

/*
 * Function: skipWindowManagerCheck
 */
bool skipWindowManagerCheck();

/*
 * Function: changeWindowManagerName
 */
const char * changeWindowManagerName();

#endif

