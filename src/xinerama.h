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

#ifndef XINERAMA_H_
#define XINERAMA_H_

#include "core.h"

/*
 * Function: is_xinerama_active
 *
 * Returns *true* if the Xinerama extension is active, *false* otherwise..
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

#endif

