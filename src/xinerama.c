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

#include "xinerama.h"
#include <stdlib.h>
#include <string.h>

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

XineramaScreenInfo * get_xinerama_screen(Display *d, int i) {
    XineramaScreenInfo *ret = malloc(sizeof(XineramaScreenInfo));
    memset(ret, 0, sizeof(XineramaScreenInfo));
    XineramaScreenInfo *xsi = XineramaQueryScreens(d, &i);
    ret->screen_number = xsi->screen_number;
    ret->x_org = xsi->x_org;
    ret->y_org = xsi->y_org;
    ret->width = xsi->width;
    ret->height = xsi->height;
    XFree(xsi);
    return ret;
}

XineramaScreenInfo * get_xinerama_screens(Display *d, int *i) {
    return XineramaQueryScreens(d, i);
}

