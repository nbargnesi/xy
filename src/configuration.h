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

/*
 * Function: get_config
 */
CONFIG * get_config(const char *);

/*
 * Function: empty_config
 */
CONFIG * empty_config();

/*
 * Function: free_config
 */
void free_config(CONFIG *);

/*
 * Function: get_config_value
 */
const char * get_config_value(CONFIG *, const char *);

/*
 * Function: set_config_value
 */
void set_config_value(CONFIG *, const char *, const char *);

/*
 * Function: write_default_config
 */
void write_default_config(const char *);

/*
 * Function: fill_config
 *
 * Inserts any missing configuration items into the provided configuration.
 */
void fill_config(CONFIG *);

/*
 * Function: configure
 *
 * Processes the application's configuration.
 */
void configure(CONFIG *);

/*
 * Function: skip_window_manager_check
 */
bool skip_window_manager_check();

/*
 * Function: change_window_manager_name
 */
const char * change_window_manager_name();

/*
 * Function: get_menu_shortcut
 */
const char * get_menu_shortcut();

/*
 * Function: get_terminal_shortcut
 */
const char * get_terminal_shortcut();

/*
 * Function: get_quit_shortcut
 */
const char * get_quit_shortcut();

/*
 * Function: get_terminal_command
 */
const char * get_terminal_command();

#endif

