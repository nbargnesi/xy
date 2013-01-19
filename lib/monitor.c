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

#include "monitor.h"
#include "xy.h"
#include "xyxlib.h"

void init_monitors(Display *d) {
    int count;
    MONITOR *monitors;
    if (is_xinerama_active(d)) {
        XineramaScreenInfo *screens = get_xinerama_screens(d, &count);
        if (!screens) return;
        monitors = calloc(count, sizeof(MONITOR));
        for (int i = 0; i < count; i++) {
            monitors[i].ordinal = screens[i].screen_number;
            monitors[i].width = screens[i].width;
            monitors[i].height = screens[i].height;
            monitors[i].xorigin = screens[i].x_org;
            monitors[i].yorigin = screens[i].y_org;
        }
        XFree(screens);
    } else {
        Screen *screen = DefaultScreenOfDisplay(d);
        if (!screen) return;
        count = 1;
        monitors = calloc(1, sizeof(MONITOR));

        memset(monitors, 0, sizeof(MONITOR));
        monitors->ordinal = 0;
        monitors->width = screen->width;
        monitors->height = screen->height;
        monitors->xorigin = 0;
        monitors->yorigin = 0;
    }

    /*
    MONITORS *ret = malloc(sizeof(MONITORS));
    memset(ret, 0, sizeof(MONITORS));
    ret->count = count;
    ret->monitors = monitors;
    return ret;
    */
}

void monitors_terminate(MONITORS *m) {
    if (m) free(m);
}

