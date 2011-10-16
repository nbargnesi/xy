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

#ifndef _XY_MONITOR_H_
#define _XY_MONITOR_H_ 1

#include "core.h"

struct Monitor {
    uint ordinal;
    uint width;
    uint height;
    uint xorigin;
    uint yorigin;
};
typedef struct Monitor MONITOR;

struct Monitors {
    uint count;
    MONITOR* monitors;
};
typedef struct Monitors MONITORS;

MONITORS * init_monitors(Display *);

#endif

