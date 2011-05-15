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

#include "lifecycle.h"
#include "xy.h"
#include "osd.h"
#include "logging.h"
#include "constants.h"
#include "broadcast.h"
#include "ipc.h"
#include "xinerama.h"

#include <dirent.h>
#include <stdio.h>
#include <fcntl.h>

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
    char *home = getenv("HOME");
    if (!home) DIE;

    CONFIG *ret = NULL;

    const int bufsize = strlen(home) + strlen(XY_CONFIG) + 2;
    char *rcpath = malloc(bufsize);
    memset(rcpath, 0, bufsize);
    strcat(rcpath, home);
    strcat(rcpath, "/");
    strcat(rcpath, XY_CONFIG);

    struct stat *st = malloc(sizeof(struct stat));;

    if (stat(rcpath, st) != 0) {
        write_default_config(rcpath);
    }

    log_info(global_log, READING_CONFIGURATION_MSG);
    ret = get_config(rcpath);
    free(st);
    free(rcpath);
    return ret;
}

void xy_startup() {
    if (!logging_init()) {
        fprintf(stderr, INIT_LOGGING_FAILURE);
        exit(1);
    }
    global_log = get_logger("xy");
    log_info(global_log, STARTUP_MSG);
    xy_dir_init();
    global_cfg = xy_rc_init();
    global_display = open_display();
    if (!global_display) {
        fprintf(stderr, "failed to open display\n");
        DIE
    }

    global_x_fd = ConnectionNumber(global_display);
    fill_config(global_cfg);
    configure(global_cfg);

    if (!ipc_init()) {
        fprintf(stderr, INIT_IPC_FAILURE);
        exit(1);
    }
    log_info(global_log, IPC_STARTUP_MSG);

    const char *group = get_config_value(global_cfg, CFG_BROADCAST_GROUP);
    const char *portstr = get_config_value(global_cfg, CFG_BROADCAST_PORT);
    const uint port = atoi(portstr);

    if (!broadcast_init(group, port)) {
        fprintf(stderr, INIT_BROADCAST_FAILURE);
        exit(1);
    }
    log_info(global_log, BROADCAST_STARTUP_MSG);

    broadcast_send(STARTUP_MSG);

    if (!is_xinerama_active(global_display)) {
        fprintf(stderr, "Xinerama is not active\n");
        DIE
    }

    global_num_screens = malloc(sizeof(uint *));
    global_screens = XineramaQueryScreens(global_display, global_num_screens);

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
     
    transition(STARTED);
}

void xy_started() {
    log_info(global_log, STARTED_MSG);
    main_loop();
}

void xy_shutting_down() {
    broadcast_send(SHUTTING_DOWN_MSG);
    log_info(global_log, SHUTTING_DOWN_MSG);

    if (global_cfg) free_config(global_cfg);
    close_display(global_display);
    close(global_x_fd);
    broadcast_terminate();
    ipc_terminate();

    transition(SHUTDOWN);
}

void xy_shutdown() {
    log_info(global_log, SHUTDOWN_MSG);
    logging_terminate();
    exit(EXIT_SUCCESS);
}

