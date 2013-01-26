/*
 * Copyright (c) 2011-2013 Nick Bargnesi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <sys/prctl.h>
#include "lifecycle.h"
#include "xy.h"
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
        write_default_config();
    }

    fprintf(stdout, "%s\n", READING_CONFIGURATION_MSG);
    ret = config_init();
    free(st);
    free(rcpath);
    return ret;
}

void xy_startup() {
    globals = malloc(sizeof(GLOBALS));    

    fprintf(stdout, "%s\n", STARTUP_MSG);
    xy_dir_init();
    globals->cfg = xy_rc_init();

    xy_inotify_init();
    if (globals->in_fd == -1) {
        DIE_MSG("xy_inotify_init failed");
    }

    if (!ipc_init()) {
        fprintf(stderr, INIT_IPC_FAILURE);
        DIE;
    }
    fprintf(stdout, "%s\n", IPC_STARTUP_MSG);

    if (!broadcast_init()) {
        fprintf(stderr, INIT_BROADCAST_FAILURE);
        DIE;
    }
    fprintf(stdout, "%s\n", BROADCAST_STARTUP_MSG);

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
    free(globals);
    restart(run_cmd);
}

void xy_shutting_down() {
    fprintf(stdout, "%s\n", SHUTTING_DOWN_MSG);
    broadcast_send(SHUTTING_DOWN_MSG);

    // Invoke cleanup functions in reverse.
    const char *fmt = "invoking shutdown hook: %s";
    for (int i = (hook_ct - 1); i >= 0; i--) {
        char *msg = malloc(strlen(fmt) + strlen(hooks[i].name));
        sprintf(msg, fmt, hooks[i].name);
        fprintf(stdout, "%s\n", msg);
        free(msg);
        hooks[i].hook();
    }
}

void xy_shutdown() {
    free(run_cmd);
    close_display(globals->dpy);
    fprintf(stdout, "%s\n", SHUTDOWN_MSG);
    exit(EXIT_SUCCESS);
}

void register_shutdown_hook(const char *name, shutdown_hook hook) {
    const char *fmt = "registered shutdown hook: %s";
    char *msg = malloc(strlen(fmt) + strlen(name));
    sprintf(msg, fmt, name);
    fprintf(stdout, "%s\n", msg);
    free(msg);
    hooks = realloc(hooks, (sizeof(SHUTDOWN_HOOK) * (hook_ct + 1)));
    hooks[hook_ct].hook = hook;
    hooks[hook_ct].name = name;
    hook_ct++;
}

