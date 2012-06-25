/*
 * Copyright (c) 2011-2012 Nick Bargnesi
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

#include "xyxlib.h"

static bool errored;

static int xerrorHandler(Display *d, XErrorEvent *ee) {
    errored  = true;
    return 0;
}

CLIENTS_LIST * cl_init() {
    CLIENTS_LIST *ret = malloc(sizeof(CLIENTS_LIST));
    memset(ret, 0, sizeof(CLIENTS_LIST));
    ret->root = malloc(sizeof(Window));
    ret->parent = malloc(sizeof(Window));
    return ret;
}

CLIENTS_LIST * get_clients(Display *d, Window w) {
    CLIENTS_LIST *ret = cl_init();

    Window *root = ret->root;
    Window *parent = ret->parent;

    Status s = XQueryTree(d, w, root, parent,
                          &ret->children,
                          &ret->num_children);

    if (!s) {
        free(ret);
        return NULL;
    }
    return ret;
}

XWindowAttributes * get_attributes(Display *d, Window w) {
    XWindowAttributes *ret = malloc(sizeof(XWindowAttributes));
    Status s = XGetWindowAttributes(d, w, ret);
    if (!s) {
        free(ret);
        return NULL;
    }
    return ret;
}

void clients_list_free(CLIENTS_LIST *cl) {
    if (!cl) return;
    free(cl->root);
    free(cl->parent);
    XFree(cl->children);
    free(cl);
}

Display * open_display() {
    return XOpenDisplay(NULL);
}
void close_display(Display *d) {
    XCloseDisplay(d);
}

ulong get_color(const char *color, Display *d, Colormap *cmap) {
    Colormap c;
    if (!cmap) {
        int screen = DefaultScreen(d);
        c = DefaultColormap(d, screen);
    } else {
        c = *cmap;
    }
    XColor xc;
    if (!XAllocNamedColor(d, c, color, &xc, &xc)) return 0L;
    return xc.pixel;
}

bool is_xinerama_active(Display *d) {
    if (XineramaIsActive(d) == True)
        return true;
    return false;
}

int get_xinerama_screen_count(Display *d) {
    int i;
    XineramaScreenInfo *xsi = XineramaQueryScreens(d, &i);
    XFree(xsi);
    return i;
}

XineramaScreenInfo * get_xinerama_screens(Display *d, int *i) {
    return XineramaQueryScreens(d, i);
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

KeySym convert_to_keysym(char *keystr) {
    return XStringToKeysym(keystr);
}

Window root_window(Display *d) {
    return DefaultRootWindow(d);
}

XSizeHints * size_hints(Display *d, Window w) {
    XSizeHints *ret = XAllocSizeHints();
    if (!ret) return NULL;
    long user_hints;
    if (!XGetWMNormalHints(d, w, ret, &user_hints)) {
        return NULL;
    }
    return ret;
}

