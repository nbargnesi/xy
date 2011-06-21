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

struct ConfigEntry {
    char *name;
    char *value;
    struct ConfigEntry *next;
};

/*
 * typedef: CONFIG_ENTRY
 *
 * Members:
 *
 *  name - the configuration entry's name
 *  value - the configuration entry's value
 *  next - the next configuration entry
 */
typedef struct ConfigEntry CONFIG_ENTRY;

struct Config {
    struct ConfigEntry *head;
    struct ConfigEntry *tail;
    uint num_entries;
};

/*
 * typedef: CONFIG
 *
 * Members:
 *
 *  head - the first configuration entry
 *  tail - the last configuration entry
 *  num_entries - the number of configuration entries present
 */
typedef struct Config CONFIG;

/*
 * Function: get_config
 *
 * Returns a <CONFIG> from a path.
 *
 * Returns:
 *
 *  <CONFIG> *
 */
CONFIG * get_config(const char *);

/*
 * Function: empty_config
 *
 * Returns an empty <CONFIG>.
 *
 * Returns:
 *
 *  <CONFIG> *
 */
CONFIG * empty_config();

/*
 * Function: free_config
 *
 * Frees the <CONFIG>.
 *
 * Parameters:
 *
 *  <CONFIG> * - the configuration to be freed
 */
void free_config(CONFIG *);

/*
 * Function: get_config_value
 *
 * Gets a configuration value (<CONFIG> value) from the <CONFIG> and name.
 *
 * Parameters:
 *
 *  <CONFIG> * - the configuration
 *  const char * - the configuration entry name
 *
 * Returns:
 *
 *  constant pointer to configuration value
 */
const char * get_config_value(CONFIG *, const char *);

/*
 * Function: set_config_value
 *
 * Parameters:
 *
 *  <CONFIG> * - the configuration
 *  const char * - the configuration entry name
 *  const char * - the configuration entry value
 */
void set_config_value(CONFIG *, const char *, const char *);

/*
 * Function: write_default_config
 *
 * Parameters:
 *
 *  const char * - the path to write the default configuration to
 */
void write_default_config(const char *);

/*
 * Function: fill_config
 *
 * Inserts any missing configuration items into the provided configuration.
 *
 * Parameters:
 *
 *  <CONFIG> * - the configuration to use
 */
void fill_config(CONFIG *);

/*
 * Function: configure
 *
 * Processes the application's configuration.
 *
 * Parameters:
 *
 *  <CONFIG> * - the configuration to use
 */
void configure(CONFIG *);

/*
 * Function: skip_window_manager_check
 *
 * Returns true if the window manager check should be skipped, false otherwise.
 *
 * Returns:
 *
 *  bool
 */
bool skip_window_manager_check();

/*
 * Function: change_window_manager_name
 *
 * Returns the string to be used as the window manager's name.
 *
 * Returns:
 *
 *  the window manager's name
 */
const char * change_window_manager_name();

/*
 * Function: get_menu_shortcut
 *
 * Returns the menu shortcut.
 *
 * Returns:
 *
 *  const char *
 */
const char * get_menu_shortcut();

/*
 * Function: get_terminal_shortcut
 *
 * Returns the terminal shortcut.
 *
 * Returns:
 *
 *  const char *
 */
const char * get_terminal_shortcut();

/*
 * Function: get_quit_shortcut
 *
 * Returns the quit shortcut.
 *
 * Returns:
 *
 *  const char *
 */
const char * get_quit_shortcut();

/*
 * Function: get_terminal_command
 *
 * Returns the terminal shortcut.
 *
 * Returns:
 *
 *  const char *
 */
const char * get_terminal_command();

#endif

