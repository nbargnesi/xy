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

#ifndef _XY_BROADCAST_H_
#define _XY_BROADCAST_H_ 1

#include "core.h"

/**
 * Define: BROADCAST_LENGTH
 *
 * Broadcast length.
 */
#define BROADCAST_LENGTH 256

bool broadcast_init();

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

