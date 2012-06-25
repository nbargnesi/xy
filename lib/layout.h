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

