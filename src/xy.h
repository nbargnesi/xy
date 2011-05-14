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
#include "ipc.h"
#include "event.h"

/*
 * Function: main
 *
 * Application entry point.
 */
int main(int, char **);

/*
 * Function: main_loop
 *
 * The main application loop.
 */
void main_loop();

/*
 * Global: global_display
 */
Display *global_display;

/*
 * Global: global_cfg
 */
CONFIG *global_cfg;

/*
 * Global: global_log
 */
log4c_category_t *global_log;

/*
 * Global: global_ipc_fd
 */
int global_ipc_fd;

/*
 * Global: global_x_fd
 */
int global_x_fd;

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
void key_pressed(XKeyEvent *);

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

#endif

