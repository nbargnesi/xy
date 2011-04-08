#include "lifecycle.h"
#include "xy.h"
#include "osd.h"
#include "logging.h"
#include "constants.h"

#include <dirent.h>
#include <stdio.h>

void module_init() {
    if (!logging_init()) {
        fprintf(stderr, INIT_LOGGING_FAILURE);
        exit(1);
    }
    osd_init();
}

void directory_init() {
    char *home = getenv("HOME");
    if (!home) DIE;

    char *path = malloc(strlen(home) + strlen(XY_DIR));
    strcat(path, home);
    strcat(path, XY_DIR);

    DIR *dir = opendir(path);
    if (dir) {
        free(dir);
        return;
    }

    log_info(xylog, CREATING_XY_DIR);
    int rc = mkdir(path, S_IRUSR | S_IWUSR | S_IXUSR);
    if (rc != 0) DIE;
}

static CONFIG * get_configuration() {
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

void setup() {
    xylog = get_logger("xy");
    global_display = open_display();
    global_cfg = get_configuration();
}

void shutdown() {
    if (global_cfg) free_config(global_cfg);
    close_display(global_display);
}

void module_terminate() {
    logging_terminate();
}

