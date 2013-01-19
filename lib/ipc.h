/*
 * Copyright (c) 2011-2013 Nick Bargnesi
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

#ifndef _XY_IPC_H_
#define _XY_IPC_H_ 1

#include "core.h"
#include "lifecycle.h"

/*
 * Enum: IPC_COMMAND
 */
enum IPC_Command {
    NO_OP,
    PING,
    QUIT,
    RESTART
};

/*
 * Typedef: COMMAND
 */
typedef enum IPC_Command COMMAND;

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
void process_ipc_buffer(char *);

/*
 * Function: convert_command_str
 */
COMMAND convert_command_str(const char *);

#endif

