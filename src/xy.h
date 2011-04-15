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

int main(int, char **);
void configure(CONFIG *);

/*! Global display. */
Display *global_display;

/*! Global configuration. */
CONFIG *global_cfg;

/*! Global logger. */
log4c_category_t *xylog;

#endif

