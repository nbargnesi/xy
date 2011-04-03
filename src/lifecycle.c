#include "lifecycle.h"
#include "xy.h"
#include "osd.h"
#include "logging.h"
#include "constants.h"

#include <stdio.h>

void module_init() {
    if (!logging_init()) {
        fprintf(stderr, INIT_LOGGING_FAILURE);
        exit(1);
    }
    osd_init();
}

static CONFIG * get_configuration() {
    CONFIG *ret = NULL;

    char *home = getenv("HOME");
    if (!home)
        goto default_configuration;

    struct stat *st = malloc(sizeof(struct stat));;
    char *path = strcat(home, CONFIG_FILE);

    if (stat(path, st) != 0) {
        goto default_configuration;
    }

    ret = get_config(path);
    free(st);
    return ret;

default_configuration:
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

