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

#ifndef _XY_CONFIGURATION_H_
#define _XY_CONFIGURATION_H_ 1

#include "types.h"

// Configuration entries as they appear in the rc
#define CFG_SKIP_WINDOW_MGR_CHECK   "skip_window_manager_check"
#define CFG_WINDOW_MGR_NAME         "window_manager_name"
#define CFG_MASTER_CLIENTS          "master_clients"
#define CFG_MASTER_PERCENTAGE       "master_percentage"
#define CFG_BROADCAST_GROUP         "broadcast_group"
#define CFG_BROADCAST_PORT          "broadcast_port"
#define CFG_TERMINAL_CMD            "terminal_command"
#define CFG_MENU_CMD                "menu_command"
#define CFG_WARP                    "warp"
#define CFG_KS_MENU                 "key_shortcut_menu"
#define CFG_KS_TERMINAL             "key_shortcut_terminal"
#define CFG_KS_QUIT                 "key_shortcut_quit"
#define CFG_KS_RESTART              "key_shortcut_restart"

#define DFLT_CFG_FILE_HDR           "# xy configuration file"
#define DFLT_SKIP_WINDOW_MGR_CHECK  false
#define DFLT_WINDOW_MGR_NAME        "xy"
#define DFLT_MASTER_CLIENTS         1
#define DFLT_MASTER_PERCENTAGE      0.50
#define DFLT_BROADCAST_GROUP        "224.0.0.3"
#define DFLT_BROADCAST_PORT         47002
#define DFLT_TERMINAL_CMD           "urxvt"
#define DFLT_MENU_CMD               "dmenu_run"
#define DFLT_WARP                   true
#define DFLT_KS_MENU                "Mod4"KS_SEPARATOR"p"
#define DFLT_KS_TERMINAL            "Mod4"KS_SEPARATOR"Return"
#define DFLT_KS_QUIT                "Mod4"KS_SEPARATOR"q"
#define DFLT_KS_RESTART             "Mod4"KS_SEPARATOR"r"

typedef struct {
    // Core
    char *term_cmd;
    char *menu_cmd;
    char *wm_name;
    bool wm_skip_check;
    bool wm_warp;
    uint wm_master_clnts;
    float wm_master_prcnt;

    // Broadcast
    uint bc_port;
    char *bc_group;

    // Key shortcuts
    char *ks_menu;
    char *ks_terminal;
    char *ks_quit;
    char *ks_restart;
} CONFIG;
CONFIG *config;

CONFIG * config_init();
void config_reinit();
void config_terminate();
void write_default_config();

#endif

