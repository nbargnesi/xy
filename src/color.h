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

#ifndef BROADCAST_H_
#define BROADCAST_H_

#include "core.h"

/**
 * Define: BROADCAST_LENGTH
 *
 * Broadcast length.
 */
#define BROADCAST_LENGTH 256

/*
 * Function: broadcast_init
 *
 * Initializes the broadcast module.
 *
 * The multicast group specified by <CFG_BROADCAST_GROUP> and
 * <CFG_BROADCAST_PORT> is joined and a thread is created to process broadcast
 * messages.
 */
bool broadcast_init(const char *, const uint);

/*
 * Function: broadcast_send
 *
 * Broadcasts a message to the multicast group.
 *
 * See <broadcast_init>.
 */
void broadcast_send(const char *);

/*
 * Function: broadcast_terminate
 *
 * Terminates the broadcast module and closes the multicast connection.
 */
void broadcast_terminate();

#endif

#ifndef COLOR_H_
#define COLOR_H_

#include "core.h"

/*! Get color. */
ulong get_color(const char *, Display *, Colormap);

#endif

