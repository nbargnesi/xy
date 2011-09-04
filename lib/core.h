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

#ifndef _CORE_H_
#define _CORE_H_

#include <errno.h>
#include <locale.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/select.h>

#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
// #include <X11/Xft/Xft.h>
#include <X11/extensions/Xinerama.h>
#include <X11/extensions/Xdamage.h>
#include <X11/extensions/Xrender.h>

#include "types.h"

void die(const char *, ...);

#define for_each_in_list(item, list) \
    for (T *item = list->head; item != NULL; item = item->next)

/*
 * Macro: XY_DIR
 *
 * The xy home directory.
 * (code)
 * .xy
 * (end)
 */
#define XY_DIR ".xy"

/*
 * Macro: IP_SOCKET_PATH
 *
 * The xy IPC path (uses <XY_DIR>).
 * (code)
 * .xy/ipc
 * (end)
 */
#define IPC_SOCKET_PATH XY_DIR"/ipc"

/*
 * Macro: XY_CONFIG
 *
 * The xy configuration path (uses <XY_DIR>).
 * (code)
 * .xy/rc
 * (end)
 */
#define XY_CONFIG XY_DIR"/rc"

/*
 * Macro: START_CLOCK
 *
 * Calls clock.
 * (code)
 * clock_t start = clock();
 * (end)
 */
#define START_CLOCK clock_t start = clock();

/*
 * Macro: STOP_CLOCK
 *
 * Calls clock.
 * (code)
 * clock_t stop = clock;
 * (end)
 */
#define STOP_CLOCK clock_t stop = clock();

/*
 * Macro: CLOCK_DELTA
 *
 * Calculate the different between calls to <START_CLOCK> and <STOP_CLOCK>.
 * (code)
 * (((double) (stop - start) ) / CLOCKS_PER_SEC)
 * (end)
 */
#define CLOCK_DELTA (((double) (stop - start) ) / CLOCKS_PER_SEC)

/*
 * Macro: SECONDS(x)
 *
 * Converts <CLOCK_DELTA> to seconds.
 * (code)
 * x * 1000000
 * (end)
 */
#define SECONDS(x) x * 1000000

/*
 * Macro: DIE
 *
 * Dies a dignified death, i.e., with function and line number.
 */
#define DIE die("DIE: %s:%d\n", __FILE__, __LINE__)

/*
 * Macro: DIE_MSG
 *
 * Dies a dignified death, i.e., with function, line number, and a suicide
 * note.
 */
#define DIE_MSG(x) die("DIE (%s): %s:%d\n", x, __FILE__, __LINE__)

/*
 * Macro: likely
 *
 * Likely branch prediction.
 */
#define likely(x) __builtin_expect(!!(x), 1)

/*
 * Macro: unlikely
 *
 * Unlikely branch prediction.
 */
#define unlikely(x) __builtin_expect(!!(x), 0)

/*
 * Define: MSG_LEN
 *
 * Length of messages sent in broadcasts and received by the IPC.
 */
#define MSG_LEN 256

#endif
