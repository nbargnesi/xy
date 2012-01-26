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

#include <sys/prctl.h>
#include "lifecycle.h"
#include "xy.h"
#include "logging.h"
#include "constants.h"
#include "broadcast.h"
#include "ipc.h"
#include "xyxlib.h"
#include "monitor.h"
#include "inotify.h"

#include <dirent.h>
#include <stdio.h>
#include <fcntl.h>

SHUTDOWN_HOOK *hooks = NULL;
int hook_ct = 0;

/*
 * Function: xy_dir_init
 *
 * Creates <XY_DIR> if needed.
 */
static void xy_dir_init() {
    char *home = getenv("HOME");
    if (!home) DIE;

    const int bufsize = strlen(home) + strlen(XY_DIR) + 2;
    char *path = malloc(bufsize);
    memset(path, 0, bufsize);
    strcat(path, home);
    strcat(path, "/");
    strcat(path, XY_DIR);

    DIR *dir = opendir(path);
    if (dir) {
        closedir(dir);
        free(path);
        return;
    }

    int rc = mkdir(path, S_IRUSR | S_IWUSR | S_IXUSR);
    if (rc != 0) DIE;
    free(path);
}

/*
 * Function: xy_rc_init
 *
 * Initializes the configuration.
 */
static CONFIG * xy_rc_init() {
    CONFIG *ret = NULL;

    char *rcpath = rc_path();

    struct stat *st = malloc(sizeof(struct stat));;

    if (stat(rcpath, st) != 0) {
        write_default_config(rcpath);
    }

    log_info(globals->log, READING_CONFIGURATION_MSG);
    ret = config_init(rcpath);
    free(st);
    free(rcpath);
    return ret;
}

void xy_startup() {
    globals = malloc(sizeof(GLOBALS));    

    if (!logging_init()) {
        fprintf(stderr, INIT_LOGGING_FAILURE);
        exit(1);
    }
    globals->log = get_logger("xy");
    log_info(globals->log, STARTUP_MSG);
    xy_dir_init();
    globals->cfg = xy_rc_init();

    globals->in_fd = xy_inotify_init();
    if (globals->in_fd == -1) {
        DIE_MSG("xy_inotify_init failed");
    }

    if (!ipc_init()) {
        fprintf(stderr, INIT_IPC_FAILURE);
        DIE;
    }
    log_info(globals->log, IPC_STARTUP_MSG);

    if (!broadcast_init()) {
        fprintf(stderr, INIT_BROADCAST_FAILURE);
        DIE;
    }
    log_info(globals->log, BROADCAST_STARTUP_MSG);

    broadcast_send(STARTUP_MSG);

    /*
    if (!is_xinerama_active(globals->dpy)) {
        fprintf(stderr, "Xinerama is not active\n");
        DIE;
    }
    */

    /*
    char buffer[MSG_LEN];
    memset(buffer, 0, MSG_LEN);
    sprintf(buffer, DISPLAYS_FOUND, *global_num_screens);
    broadcast_send(buffer);
    memset(buffer, 0, MSG_LEN);

    for (int i = 0; i < *global_num_screens; i++) {
        int sn = global_screens[i].screen_number;
        int xorg = global_screens[i].x_org;
        int yorg = global_screens[i].y_org;
        int width = global_screens[i].width;
        int height = global_screens[i].height;
        sprintf(buffer, DISPLAY_MESSAGE, sn, xorg, yorg, width, height);
        broadcast_send(buffer);
        memset(buffer, 0, MSG_LEN);
    }
    */
    
    // TODO grab keys
     
    xy_init();
    set_process_name("xy: main");
}

void xy_restart() {
    restart(run_cmd);
}

void xy_shutting_down() {
    log_info(globals->log, SHUTTING_DOWN_MSG);
    broadcast_send(SHUTTING_DOWN_MSG);

    // Invoke cleanup functions in reverse.
    const char *fmt = "invoking shutdown hook: %s";
    for (int i = (hook_ct - 1); i >= 0; i--) {
        char *msg = malloc(strlen(fmt) + strlen(hooks[i].name));
        sprintf(msg, fmt, hooks[i].name);
        log_info(globals->log, msg);
        free(msg);
        hooks[i].hook();
    }
}

void xy_shutdown() {
    free(run_cmd);
    close_display(globals->dpy);
    log_info(globals->log, SHUTDOWN_MSG);
    logging_terminate();
    exit(EXIT_SUCCESS);
}

void register_shutdown_hook(const char *name, shutdown_hook hook) {
    const char *fmt = "registered shutdown hook: %s";
    char *msg = malloc(strlen(fmt) + strlen(name));
    sprintf(msg, fmt, name);
    log_info(globals->log, msg);
    free(msg);
    hooks = realloc(hooks, (sizeof(SHUTDOWN_HOOK) * (hook_ct + 1)));
    hooks[hook_ct].hook = hook;
    hooks[hook_ct].name = name;
    hook_ct++;
}

