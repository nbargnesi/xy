#ifndef _XY_H_
#define _XY_H_

#include "core.h"
#include "constants.h"
#include "util.h"
#include "color.h"
#include "display.h"
#include "restart.h"
#include "screen.h"
#include "wmcheck.h"
#include "xinerama.h"
#include "logging.h"
#include "configuration.h"
#include "xyxlib.h"
#include "sysinfo.h"
#include "state.h"
#include "broadcast.h"

/**
 * Function: main
 *
 * Application entry point.
 */
int main(int, char **);

/**
 * Function: configure
 *
 * Processes the application's configuration.
 */
void configure(CONFIG *);

/**
 * Function: main_loop
 *
 * The main application loop.
 */
void main_loop();

Display *global_display;

CONFIG *global_cfg;

log4c_category_t *xylog;

/**
 * Global: ipc_fd
 */
int ipc_fd;

#endif

