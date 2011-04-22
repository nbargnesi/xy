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

#include "screen.h"
#include <stdlib.h>
#include <stdio.h>

SCREEN * default_screen(Display *d) {
    const int default_screen = DefaultScreen(d);
    SCREEN *screen = malloc(sizeof(SCREEN));
    screen->number = default_screen;
    screen->root = RootWindow(d, default_screen);
    screen->width = DisplayWidth(d, default_screen);
    screen->height = DisplayHeight(d, default_screen);
    screen->depth = DefaultDepth(d, default_screen);
    screen->visual = DefaultVisual(d, default_screen);
    screen->colormap = DefaultColormap(d, default_screen);
    return screen;
}

