#include "startup.h"
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

void module_terminate() {
    logging_terminate();
}

