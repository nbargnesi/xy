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

#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <X11/Xft/Xft.h>
#include <X11/extensions/Xinerama.h>
#include <X11/extensions/Xdamage.h>
#include <X11/extensions/Xrender.h>

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
#define DIE fprintf(stderr, "DIE %s:%d\n", __func__, __LINE__)

#endif
