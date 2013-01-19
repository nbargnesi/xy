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

#include "state.h"
#include "lifecycle.h"
#include "xy.h"

void transition(const STATE state) {
    switch (state) {
        case STATE_INIT:
            xy_startup();
            transition(STATE_RUNNING);
            break;
        case STATE_RUNNING:
            main_loop();
            break;
        case STATE_RESTARTING:
            xy_shutting_down();
            xy_restart();
            break;
        case STATE_SHUTTING_DOWN:
            xy_shutting_down();
            transition(STATE_SHUTDOWN);
            break;
        case STATE_SHUTDOWN:
            xy_shutdown();
            break;
    }
}

