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

#ifndef _XY_CORE_H_
#define _XY_CORE_H_ 1

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
#include <X11/XKBlib.h>
// #include <X11/Xft/Xft.h>
#include <X11/extensions/Xinerama.h>
#include <X11/extensions/Xdamage.h>
#include <X11/extensions/Xrender.h>

#include "types.h"

Display *dpy;

void die(const char *, ...);

#define for_each_in_list(item, list) \
    for (T *item = list->head; item != NULL; item = item->next)

#define min(x, y) ((x < y) ? x : y)


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

char *run_cmd;

#endif
