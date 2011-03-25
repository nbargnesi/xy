#include "wmcheck.h"

static bool errored;

static int xerrorHandler(Display *d, XErrorEvent *ee) {
    errored  = true;
    return 0;
}

bool is_window_manager_running(Display *d) {
    errored = false;
    int (*currentHandler)(Display *, XErrorEvent *);
    currentHandler = XSetErrorHandler(xerrorHandler);
    XSelectInput(d, DefaultRootWindow(d), SubstructureRedirectMask);
    XSync(d, False);
    XSetErrorHandler(currentHandler);
    if (errored) return true;
    return false;
}

