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

#ifndef EVENT_H_
#define EVENT_H_

#include "core.h"

/*
 * Function: event_handler
 */
void event_handler(XEvent *);

/*
 * Function: is_control_pressed
 */
bool is_control_pressed(XKeyEvent *);

/*
 * Function: is_shift_pressed
 */
bool is_shift_pressed(XKeyEvent *);

/*
 * Function: is_mod1_pressed
 */
bool is_mod1_pressed(XKeyEvent *);

/*
 * Function: is_mod2_pressed
 */
bool is_mod2_pressed(XKeyEvent *);

/*
 * Function: is_mod3_pressed
 */
bool is_mod3_pressed(XKeyEvent *);

/*
 * Function: is_mod4_pressed
 */
bool is_mod4_pressed(XKeyEvent *);

/*
 * Function: is_ks_pressed
 *
 * Returns *true* if the key shortcut is being pressed in the key event,
 * *false* otherwise.
 */
bool is_ks_pressed(const char *, XKeyEvent *);

#endif

