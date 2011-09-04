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

#include "monitor.h"
#include "xy.h"
#include "xyxlib.h"

MONITORS * init_monitors(Display *d) {
    int count;
    MONITOR *monitors;
    if (is_xinerama_active(d)) {
        XineramaScreenInfo *screens = get_xinerama_screens(d, &count);
        if (!screens) return NULL;
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
        if (!screen) return NULL;
        count = 1;
        monitors = calloc(1, sizeof(MONITOR));

        memset(monitors, 0, sizeof(MONITOR));
        monitors->ordinal = 0;
        monitors->width = screen->width;
        monitors->height = screen->height;
        monitors->xorigin = 0;
        monitors->yorigin = 0;
    }

    MONITORS *ret = malloc(sizeof(MONITORS));
    memset(ret, 0, sizeof(MONITORS));
    ret->count = count;
    ret->monitors = monitors;
    return ret;
}

