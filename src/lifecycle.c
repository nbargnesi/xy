#include "lifecycle.h"
#include "xy.h"
#include "osd.h"
#include "logging.h"
#include "constants.h"

#include <dirent.h>
#include <stdio.h>

/*
 * Function: xy_dir_init
 *
 * Creates <XY_DIR> if needed.
 */
static void xy_dir_init() {
    char *home = getenv("HOME");
    if (!home) DIE;

    const int bufsize = strlen(home) + strlen(XY_DIR) + 1;
    char *path = malloc(bufsize);
    memset(path, 0, bufsize);
    strcat(path, home);
    strcat(path, "/");
    strcat(path, XY_DIR);

    DIR *dir = opendir(path);
    if (dir) {
        free(dir);
        return;
    }

    int rc = mkdir(path, S_IRUSR | S_IWUSR | S_IXUSR);
    if (rc != 0) DIE;
}

/*
 * Function: xy_rc_init
 *
 * Initializes the configuration.
 */
static CONFIG * xy_rc_init() {
    CONFIG *ret = NULL;

    char *home = getenv("HOME");
    if (!home) goto default_configuration;

    char *rcpath = malloc(strlen(home) + strlen(XY_CONFIG));
    strcat(rcpath, home);
    strcat(rcpath, XY_CONFIG);

    struct stat *st = malloc(sizeof(struct stat));;

    if (stat(rcpath, st) != 0) {
        goto default_configuration;
    }

    log_info(xylog, READING_CONFIGURATION_MSG);
    ret = get_config(rcpath);
    free(st);
    return ret;

default_configuration:
    log_info(xylog, USING_DEFAULT_CONFIGURATION_MSG);
    ret = empty_config();
    return ret;
}

/*
 * Function: ipc_init
 *
 * Creates IPC_SOCKET_PATH.
 */
static void ipc_init() {
    // TODO
}

void startup() {
    if (!logging_init()) {
        fprintf(stderr, INIT_LOGGING_FAILURE);
        exit(1);
    }
    xylog = get_logger("xy");
    log_info(xylog, STARTUP_MSG);
    xy_dir_init();
    global_cfg = xy_rc_init();
    ipc_init();
    global_display = open_display();
    transition(STARTED);
}

void started() {
    log_info(xylog, STARTED_MSG);
}

void shutting_down() {
    log_info(xylog, SHUTTING_DOWN_MSG);
    transition(SHUTDOWN);
}

void shutdown() {
    log_info(xylog, SHUTDOWN_MSG);
    if (global_cfg) free_config(global_cfg);
    close_display(global_display);
}

void module_init() {
    osd_init();
}

void module_terminate() {
    logging_terminate();
}

