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

#include "constants.h"
#include "xy.h"
#include "configuration.h"
#include "util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void set_config_value(CONFIG *cfg, const char *name, const char *value) {
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
}

void write_default_config(const char *rcpath) {
    FILE *cfg = fopen(rcpath, "w");
    // XXX: validate cfg

    fprintf(cfg, "%s\n", DEFAULT_CFG_FILE_HDR);

    fprintf(cfg, "%s = ", CFG_SKIP_WINDOW_MGR_CHECK);
    fprintf(cfg, "%s\n", DEFAULT_SKIP_WINDOW_MGR_CHECK);

    fprintf(cfg, "%s = ", CFG_WINDOW_MGR_NAME);
    fprintf(cfg, "%s\n", DEFAULT_WINDOW_MGR_NAME);

    fprintf(cfg, "%s = ", CFG_BROADCAST_GROUP);
    fprintf(cfg, "%s\n", DEFAULT_BROADCAST_GROUP);

    fprintf(cfg, "%s = ", CFG_BROADCAST_PORT);
    fprintf(cfg, "%s\n", DEFAULT_BROADCAST_PORT);

    fprintf(cfg, "%s = ", CFG_KS_QUIT);
    fprintf(cfg, "%s\n", DEFAULT_KS_QUIT);

    fprintf(cfg, "%s = ", CFG_KS_TERMINAL);
    fprintf(cfg, "%s\n", DEFAULT_KS_TERMINAL);

    fprintf(cfg, "%s = ", CFG_KS_MENU);
    fprintf(cfg, "%s\n", DEFAULT_KS_MENU);

    fprintf(cfg, "%s = ", CFG_TERMINAL_COMMAND);
    fprintf(cfg, "%s\n", DEFAULT_TERMINAL_COMMAND);

    fclose(cfg);
}

void fill_config(CONFIG *cfg) {

    char *name = CFG_SKIP_WINDOW_MGR_CHECK;
    char *value = DEFAULT_SKIP_WINDOW_MGR_CHECK;
    if (!get_config_value(global_cfg, name)) {
        set_config_value(global_cfg, name, value);
    }

    name = CFG_WINDOW_MGR_NAME;
    value = DEFAULT_WINDOW_MGR_NAME;
    if (!get_config_value(global_cfg, name)) {
        set_config_value(global_cfg, name, value);
    }

    name = CFG_BROADCAST_GROUP;
    value = DEFAULT_BROADCAST_GROUP;
    if (!get_config_value(global_cfg, name)) {
        set_config_value(global_cfg, name, value);
    }

    name = CFG_BROADCAST_PORT;
    value = DEFAULT_BROADCAST_PORT;
    if (!get_config_value(global_cfg, name)) {
        set_config_value(global_cfg, name, value);
    }

    name = CFG_KS_MENU;
    value = DEFAULT_KS_MENU;
    if (!get_config_value(global_cfg, name)) {
        set_config_value(global_cfg, name, value);
    }

    name = CFG_KS_TERMINAL;
    value = DEFAULT_KS_TERMINAL;
    if (!get_config_value(global_cfg, name)) {
        set_config_value(global_cfg, name, value);
    }

    name = CFG_KS_QUIT;
    value = DEFAULT_KS_QUIT;
    if (!get_config_value(global_cfg, name)) {
        set_config_value(global_cfg, name, value);
    }

    name = CFG_TERMINAL_COMMAND;
    value = DEFAULT_TERMINAL_COMMAND;
    if (!get_config_value(global_cfg, name)) {
        set_config_value(global_cfg, name, value);
    }

    for (CONFIG_ENTRY *c = cfg->head; c; c = c->next) {
        const char *name = c->name;
        const char *value = c->value;
        const size_t length = strlen(name) + strlen(value) + 3;
        char *buffer = malloc(length);
        sprintf(buffer, "%s: %s", name, value);
        log_debug(global_log, buffer);
        free(buffer);
    }
}

void configure(CONFIG *cfg) {
    // Should we skip the window manager check?
    if (!skip_window_manager_check()) {
        if (is_window_manager_running(global_display)) {
            log_fatal(global_log, WINDOW_MGR_RUNNING);
        }
    } else
        log_info(global_log, SKIP_WINDOW_MGR_CHECK_MSG);

    // Should we change the window manager's name?
    const char *wmname = change_window_manager_name();
    if (wmname) {
        log_info(global_log, CHANGE_WINDOW_MGR_NAME_MSG);
        change_name(global_display, wmname);
    }
}

bool skip_window_manager_check() {
    const char *wmcheck = get_config_value(global_cfg, CFG_SKIP_WINDOW_MGR_CHECK);
    if (!wmcheck || streq(wmcheck, "true"))
        return true;
    return false;
}

const char * change_window_manager_name() {
    const char *wmname = get_config_value(global_cfg, CFG_WINDOW_MGR_NAME);
    return wmname;
}

const char * get_menu_shortcut() {
    const char *ks = get_config_value(global_cfg, CFG_KS_MENU);
    return ks;
}

const char * get_terminal_shortcut() {
    const char *ks = get_config_value(global_cfg, CFG_KS_TERMINAL);
    return ks;
}

const char * get_quit_shortcut() {
    const char *ks = get_config_value(global_cfg, CFG_KS_QUIT);
    return ks;
}

const char * get_terminal_command() {
    const char *cmd = get_config_value(global_cfg, CFG_TERMINAL_COMMAND);
    return cmd;
}

