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
 * The xy home directory: ".xy"
 */
#define XY_DIR ".xy"

/*
 * Macro: IP_SOCKET_PATH
 *
 * The xy IPC path: ".xy/ipc"
 */
#define IPC_SOCKET_PATH XY_DIR"/ipc"

/*
 * Macro: XY_CONFIG
 *
 * The xy configuration path: ".xy/rc"
 */
#define XY_CONFIG XY_DIR"/rc"

#define START_CLOCK clock_t start = clock();
#define STOP_CLOCK clock_t stop = clock();
#define CLOCK_DELTA (((double) (stop - start) ) / CLOCKS_PER_SEC)

#define SECONDS(x) x * 1000000

#define DIE fprintf(stderr, "DIE %s:%d\n", __func__, __LINE__)

#endif
