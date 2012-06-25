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

#ifndef _XY_XYXLIB_H_
#define _XY_XYXLIB_H_ 1

#include "core.h"

typedef struct {
    Window *root;
    Window *parent;
    Window *children;
    uint num_children;
} CLIENTS_LIST;

CLIENTS_LIST * get_clients(Display *, Window);
XWindowAttributes * get_attributes(Display *, Window);
void clients_list_free(CLIENTS_LIST *);
Display * open_display();
void close_display(Display *);

/*
 * Function: get_color
 */
ulong get_color(const char *, Display *, Colormap *);

/*
 * Function: is_xinerama_active
 *
 * Returns *true* if the Xinerama extension is active, *false* otherwise.
 */
bool is_xinerama_active(Display *);

/*
 * Function: get_xinerama_screen_count
 *
 * Returns the number of Xinerama screens.
 */
int get_xinerama_screen_count(Display *);

/*
 * Function: get_xinerama_screens
 *
 * Returns the Xinerama screens for the supplied display, setting the number
 * of displays to the integer pointed to by the second argument.
 */
XineramaScreenInfo * get_xinerama_screens(Display *, int *);

bool is_window_manager_running(Display *);

KeySym convert_to_keysym(char *);

Window root_window(Display *);

XSizeHints * size_hints(Display *, Window);

#endif

