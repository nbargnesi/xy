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

