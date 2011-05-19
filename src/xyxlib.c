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

#include "xyxlib.h"

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

    Status s = XQueryTree(d, w, root, parent, &ret->children, &ret->num_children);
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

