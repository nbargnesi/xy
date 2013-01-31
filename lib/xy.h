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

#ifndef _XY_H_
#define _XY_H_ 1

#include "core.h"
#include "constants.h"
#include "util.h"
#include "xyxlib.h"
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
    int ipc_fd;
    int x_fd;
    int in_fd;
    XineramaScreenInfo *screens;
    int num_screens;
    int dflt_scrn;
} GLOBALS;
GLOBALS *globals;

struct sigaction *_sighndlr;

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

