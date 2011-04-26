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

#include "constants.h"
#include "wmcheck.h"
#include "xy.h"
#include "configuration.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

CONFIG * get_config(const char *pathname) {

    FILE *fp = fopen(pathname, "r");
    if (!fp) return NULL;

    char *line = NULL;
    char *name = NULL;
    char *value = NULL;
    size_t len = 0;

    CONFIG *cfg = empty_config();
    while (getline(&line, &len, fp) != -1) {

        // Tokenize the line using '=' as the delimiter.
        char *token = strtok(line, "=");
        if (!token) continue;

        // Trim leading/trailing whitespace.
        name = trim(token);

        // Skip comments
        if (*name == '#') continue;
        token = strtok(NULL, "=");
        if (!token) continue;
        value = trim(token);

        // Create a CONFIG_ENTRY for the name/value tokens.
        CONFIG_ENTRY *ce = malloc(sizeof(CONFIG_ENTRY));
        memset(ce, 0, sizeof(CONFIG_ENTRY));
        ce->name = strdup(name);
        ce->value = strdup(value);
        ce->next = NULL;

        // If no entries exist...
        if (cfg->num_entries == 0) {
            // ce becomes head and tail
            cfg->head = ce;
            cfg->tail = ce;
        } else {
            // ce becomes tail
            cfg->tail->next = ce;
            cfg->tail = ce;
        }
        cfg->num_entries++;
    }

    free(line);
    fclose(fp);
    return cfg;
}

CONFIG * empty_config() {
    CONFIG *cfg = malloc(sizeof(CONFIG));
    memset(cfg, 0, sizeof(CONFIG));
    cfg->head = NULL;
    cfg->tail = NULL;
    cfg->num_entries = 0;
    return cfg;
}

void free_config(CONFIG *cfg) {
    if (cfg) {
        for (CONFIG_ENTRY *c = cfg->head; c; ) {
            free(c->name);
            free(c->value);
            CONFIG_ENTRY *next = c->next;
            free(c);
            c = next;
        }
        free(cfg);
    }
}

const char *get_config_value(CONFIG *cfg, const char *name) {
    for (CONFIG_ENTRY *c = cfg->head; c; c = c->next) {
        if (streq(c->name, name)) return c->value;
    }
    return NULL;
}

void configure(CONFIG *cfg) {
    // Should we skip the window manager check?
    if (!skipWindowManagerCheck()) {
        if (is_window_manager_running(global_display)) {
            log_fatal(global_log, WINDOW_MGR_RUNNING);
        }
    } else
        log_info(global_log, SKIP_WINDOW_MGR_CHECK_MSG);

    // Should we change the window manager's name?
    const char *wmname = changeWindowManagerName();
    if (wmname) {
        log_info(global_log, CHANGE_WINDOW_MGR_NAME_MSG);
        change_name(global_display, wmname);
    }
}

bool skipWindowManagerCheck() {
    const char *wmcheck = get_config_value(global_cfg, CFG_SKIP_WINDOW_MGR_CHECK);
    if (!wmcheck || streq(wmcheck, "true"))
        return true;
    return false;
}

const char * changeWindowManagerName() {
    const char *wmname = get_config_value(global_cfg, CFG_WINDOW_MGR_NAME);
    return wmname;
}

