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
#include "lifecycle.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static CONFIG * default_config();
static void process_entry(char *, char*);
static bool get_boolean(char *);
static uint get_uint(char *);
static float get_float(char *);
static void read_config(const char *);

CONFIG * config_init() {
    config = default_config();
    char *rcpath = rc_path();
    read_config(rcpath);
    free(rcpath);
    register_shutdown_hook("config", config_terminate);
    return config;
}

void config_reinit() {
    char *rcpath = rc_path();
    read_config(rcpath);
    free(rcpath);
}

void config_terminate() {
    if (config) {
        free(config->term_cmd);
        free(config->menu_cmd);
        free(config->wm_name);
        free(config->bc_group);
        free(config->ks_menu);
        free(config->ks_terminal);
        free(config->ks_quit);
        free(config->ks_restart);
        free(config);
    }
}

void write_default_config() {
    char *rcpath = rc_path();
    FILE *cfg = fopen(rcpath, "w");
    free(rcpath);

    fprintf(cfg, "%s\n", DFLT_CFG_FILE_HDR);

    fprintf(cfg, "%s = ", CFG_SKIP_WINDOW_MGR_CHECK);
    fprintf(cfg, "false\n");

    fprintf(cfg, "%s = ", CFG_WINDOW_MGR_NAME);
    fprintf(cfg, "%s\n", DFLT_WINDOW_MGR_NAME);

    fprintf(cfg, "%s = ", CFG_MASTER_CLIENTS);
    fprintf(cfg, "%d\n", DFLT_MASTER_CLIENTS);

    fprintf(cfg, "%s = ", CFG_MASTER_PERCENTAGE);
    fprintf(cfg, "%f\n", DFLT_MASTER_PERCENTAGE);

    fprintf(cfg, "%s = ", CFG_WARP);
    fprintf(cfg, "true\n");

    fprintf(cfg, "%s = ", CFG_BROADCAST_GROUP);
    fprintf(cfg, "%s\n", DFLT_BROADCAST_GROUP);

    fprintf(cfg, "%s = ", CFG_BROADCAST_PORT);
    fprintf(cfg, "%d\n", DFLT_BROADCAST_PORT);

    fprintf(cfg, "%s = ", CFG_KS_QUIT);
    fprintf(cfg, "%s\n", DFLT_KS_QUIT);

    fprintf(cfg, "%s = ", CFG_KS_TERMINAL);
    fprintf(cfg, "%s\n", DFLT_KS_TERMINAL);

    fprintf(cfg, "%s = ", CFG_KS_MENU);
    fprintf(cfg, "%s\n", DFLT_KS_MENU);

    fprintf(cfg, "%s = ", CFG_KS_RESTART);
    fprintf(cfg, "%s\n", DFLT_KS_RESTART);

    fprintf(cfg, "%s = ", CFG_TERMINAL_CMD);
    fprintf(cfg, "%s\n", DFLT_TERMINAL_CMD);

    fprintf(cfg, "%s = ", CFG_MENU_CMD);
    fprintf(cfg, "%s\n", DFLT_MENU_CMD);

    fclose(cfg);
}

static CONFIG * default_config() {
    CONFIG *cfg = malloc(sizeof(CONFIG));
    memset(cfg, 0, sizeof(CONFIG));

    cfg->wm_master_clnts = DFLT_MASTER_CLIENTS;
    cfg->wm_master_prcnt = DFLT_MASTER_PERCENTAGE;
    cfg->term_cmd = strdup(DFLT_TERMINAL_CMD);
    cfg->menu_cmd = strdup(DFLT_MENU_CMD);
    cfg->wm_name = strdup(DFLT_WINDOW_MGR_NAME);
    cfg->wm_skip_check = DFLT_SKIP_WINDOW_MGR_CHECK;
    cfg->wm_warp = DFLT_WARP;
    cfg->bc_port = DFLT_BROADCAST_PORT;
    cfg->bc_group = strdup(DFLT_BROADCAST_GROUP);
    cfg->ks_menu = strdup(DFLT_KS_MENU);
    cfg->ks_terminal = strdup(DFLT_KS_TERMINAL);
    cfg->ks_quit = strdup(DFLT_KS_QUIT);
    cfg->ks_restart = strdup(DFLT_KS_RESTART);

    return cfg;
}

static void read_config(const char *rcpath) {
    FILE *fp = fopen(rcpath, "r");
    if (!fp) return;

    char *line = NULL;
    char *name = NULL;
    char *value = NULL;
    size_t len = 0;

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

        name = strdup(name);
        value = strdup(value);

        process_entry(name, value);
        free(name);
    }

    free(line);
    fclose(fp);
}

static void process_entry(char *name, char *value) {
    if (streq(name, CFG_SKIP_WINDOW_MGR_CHECK)) {
        config->wm_skip_check = get_boolean(value);
        free(value);
    } else if (streq(name, CFG_WINDOW_MGR_NAME)) {
        free(config->wm_name);
        config->wm_name = value;
    } else if (streq(name, CFG_MASTER_CLIENTS)) {
        config->wm_master_clnts = get_uint(value);
        free(value);
    } else if (streq(name, CFG_MASTER_PERCENTAGE)) {
        float f = get_float(value);
        free(value);
        if (f < 0.1) f = 0.1;
        if (f > 0.9) f = 0.9;
        config->wm_master_prcnt = f;
    } else if (streq(name, CFG_BROADCAST_GROUP)) {
        free(config->bc_group);
        config->bc_group = value;
    } else if (streq(name, CFG_BROADCAST_PORT)) {
        config->bc_port = get_uint(value);
        free(value);
    } else if (streq(name, CFG_TERMINAL_CMD)) {
        free(config->term_cmd);
        config->term_cmd = value;
    } else if (streq(name, CFG_MENU_CMD)) {
        free(config->menu_cmd);
        config->menu_cmd = value;
    } else if (streq(name, CFG_WARP)) {
        config->wm_warp = get_boolean(value);
        free(value);
    } else if (streq(name, CFG_KS_MENU)) {
        free(config->ks_menu);
        config->ks_menu = value;
    } else if (streq(name, CFG_KS_TERMINAL)) {
        free(config->ks_terminal);
        config->ks_terminal = value;
    } else if (streq(name, CFG_KS_QUIT)) {
        free(config->ks_quit);
        config->ks_quit = value;
    } else if (streq(name, CFG_KS_RESTART)) {
        free(config->ks_restart);
        config->ks_restart = value;
    } else {
        fprintf(stderr, "unknown configuration %s\n", name);
    }
}

static bool get_boolean(char *c) {
    if (streq(c, "true")) return true;
    return false;
}

static uint get_uint(char *c) {
    return (uint) atoi(c);
}

static float get_float(char *c) {
    return (float) atof(c);
}

