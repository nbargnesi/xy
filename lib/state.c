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

#include "state.h"
#include "lifecycle.h"
#include "xy.h"

void transition(const STATE state) {
    switch (state) {
        case STATE_INIT:
            xy_startup();
            xy_started();
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

