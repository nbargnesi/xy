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

#ifndef _XY_STATE_H_
#define _XY_STATE_H_ 1

enum StateEnum {
    STATE_INIT,
    STATE_RUNNING,
    STATE_RESTARTING,
    STATE_SHUTTING_DOWN,
    STATE_SHUTDOWN
};
typedef enum StateEnum STATE;

/*
 * Transitions to a state.
 */
void transition(const STATE);

#endif
