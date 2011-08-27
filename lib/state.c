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

#include "state.h"
#include "lifecycle.h"

void transition(const State state) {
    switch (state) {
        case STARTING_UP:
            xy_startup();
            break;
        case STARTED:
            xy_started();
            break;
        case SHUTTING_DOWN:
            xy_shutting_down();
            break;
        case SHUTDOWN:
            xy_shutdown();
            break;
    }
}

