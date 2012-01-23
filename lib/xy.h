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

#ifndef _XY_H_
#define _XY_H_ 1

#include "core.h"
#include "constants.h"
#include "util.h"
#include "xyxlib.h"
#include "logging.h"
#include "configuration.h"
#include "xyxlib.h"
#include "sysinfo.h"
#include "state.h"
#include "broadcast.h"
#include "ipc.h"
#include "event.h"
#include "monitor.h"
#include "inotify.h"

typedef struct {
    CONFIG *cfg;
    Display *dpy;
    log4c_category_t *log; 
    int ipc_fd;
    int x_fd;
    int in_fd;
    XineramaScreenInfo *screens;
    int num_screens;
    int dflt_scrn;
} GLOBALS;
GLOBALS *globals;

/*
 * Function: main
 *
 * Application entry point.
 */
int main(int, char **);

void xy_init();

/*
 * Function: main_loop
 *
 * The main application loop.
 */
void main_loop();

/*
 * Function: ipc_quit
 */
void ipc_quit();

/*
 * Function: ipc_ping
 */
void ipc_ping();

/*
 * Function: key_pressed
 */
bool key_pressed(XKeyEvent *);

/*
 * Function: key_released
 */
void key_released(XKeyEvent *);

/*
 * Function: configure_request
 */
void configure_request(XConfigureRequestEvent *);

/*
 * Function: configure_notify
 */
void configure_notify(XConfigureEvent *);

/*
 * Function: destroy_notify
 */
void destroy_notify(XDestroyWindowEvent *);

/*
 * Function: enter_notify
 */
void enter_notify(XCrossingEvent *);

/*
 * Function: expose
 */
void expose(XExposeEvent *);

/*
 * Function: focus_event
 */
void focus_event(XFocusChangeEvent *);

/*
 * Function: mapping_notify
 */
void mapping_notify(XMappingEvent *);

/*
 * Function: map_request
 */
void map_request(XMapRequestEvent *);

/*
 * Function: property_notify
 */
void property_notify(XPropertyEvent *);

/*
 * Function: unmap_notify
 */
void unmap_notify(XUnmapEvent *);

void xy_cleanup();

#endif

