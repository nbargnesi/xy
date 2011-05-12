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

static void write_default_config(const char *rcpath) {
    FILE *cfg = fopen(rcpath, "w");

    fprintf(cfg, "%s\n", DEFAULT_CFG_FILE_HDR);

    fprintf(cfg, "%s = ", CFG_SKIP_WINDOW_MGR_CHECK);
    fprintf(cfg, "%s\n", DEFAULT_SKIP_WINDOW_MGR_CHECK);

    fprintf(cfg, "%s = ", CFG_WINDOW_MGR_NAME);
    fprintf(cfg, "%s\n", DEFAULT_WINDOW_MGR_NAME);

    fprintf(cfg, "%s = ", CFG_BROADCAST_GROUP);
    fprintf(cfg, "%s\n", DEFAULT_BROADCAST_GROUP);

    fprintf(cfg, "%s = ", CFG_BROADCAST_PORT);
    fprintf(cfg, "%s\n", DEFAULT_BROADCAST_PORT);

    fclose(cfg);
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

