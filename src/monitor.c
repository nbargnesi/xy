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
    if (is_xinerama_active(d)) {
        int count;
        XineramaScreenInfo *screens = get_xinerama_screens(d, &count);
        if (!screens) return NULL;
        return NULL;
    } else {
        Screen *screen = DefaultScreenOfDisplay(d);
        if (!screen) return NULL;

        MONITOR *m = malloc(sizeof(MONITOR));
        memset(m, 0, sizeof(MONITOR));
        m->ordinal = 0;
        m->width = screen->width;
        m->height = screen->height;
        m->xorigin = 0;
        m->yorigin = 0;

        MONITORS *ms = malloc(sizeof(MONITORS));
        memset(ms, 0, sizeof(MONITORS));
        ms->count = 1;
        ms->monitors = calloc(1, sizeof(MONITOR));
        ms->monitors = m;
        return ms;
    }
}

