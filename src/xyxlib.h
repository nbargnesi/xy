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

#ifndef XYXLIB_H_
#define XYXLIB_H_

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

#endif
