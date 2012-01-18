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

#ifndef _XY_LAYOUT_H_
#define _XY_LAYOUT_H_ 1

#include "core.h"

/*
 * Layout modes.
 */
enum Layout {

    /*
     * Three rows. The middle row is the master client.
     */
    PANORAMA,

    /*
     * Three columns. The middle column is the master client.
     */
    VISTA,

    /*
     * An NxM grid. The master client is centered above other clients and
     * appears focused with gaps around it.
     */
    FOCUS,

    /*
     * An NxM grid.
     */
    GRID,

    /*
     * Clients appear as one of any number of uniformly sized rows.
     */
    ROWS,

    /*
     * Clients appear as one of any number of uniformly sized columns.
     */
    COLUMNS,

    /*
     * Row-based. The master client appears in the northern region.
     */
    NORTH,

    /*
     * Row-based. The master client appears in the southern region.
     */
    SOUTH,

    /*
     * Two columns. The master client appears in the eastern region.
     */
    EAST,

    /*
     * Two columns. The master client appears in the western region.
     */
    WEST
};
typedef enum Layout LAYOUT;

typedef struct {
    uint x;
    uint y;
    uint width;
    uint height;
} LOCATION;

LOCATION * layout(LAYOUT, XSizeHints *);

#endif

