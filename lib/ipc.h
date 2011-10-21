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

#ifndef _XY_IPC_H_
#define _XY_IPC_H_ 1

#include "core.h"
#include "lifecycle.h"

/*
 * Enum: IPC_COMMAND
 */
enum IPC_COMMAND {
    NO_OP,
    PING,
    QUIT,
    RESTART
};

/*
 * Typedef: Command
 */
typedef enum IPC_COMMAND Command;

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

/*
 * Function: process_ipc_buffer
 */
void process_ipc_buffer(const char *);

/*
 * Function: convert_command_str
 */
Command convert_command_str(const char *);

#endif

