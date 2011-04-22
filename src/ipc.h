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

#ifndef IPC_H_
#define IPC_H_

#include "core.h"

/**
 * Define: IPC_MSG_LENGTH
 *
 * IPC message length.
 */
#define IPC_MSG_LENGTH 256

/*
 * Function: ipc_init
 *
 * Initializes the IPC module.
 */
bool ipc_init();

/*
 * Function: broadcast_terminate
 *
 * Terminates the broadcast module and closes the multicast connection.
 */
void ipc_terminate();

#endif

