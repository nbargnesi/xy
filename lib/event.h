/*
 * Copyright (c) 2011-2012 Nick Bargnesi
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

#ifndef _XY_EVENT_H_
#define _XY_EVENT_H_ 1

#include "core.h"

/*
 * Function: event_handler
 *
 * Invokes the appropriate xy callback.
 *
 * Parameters:
 *
 *  XEvent * - pointer to an XEvent
 */
void event_handler(XEvent *);

/*
 * Function: is_control_pressed
 *
 * Returns true if control is pressed in the key event, false otherwise.
 *
 * Parameters:
 *
 *  XKeyEvent * - pointer to XKeyEvent
 *
 * Returns:
 *
 *  bool
 */
bool is_control_pressed(XKeyEvent *);

/*
 * Function: is_shift_pressed
 *
 * Returns true if shift is pressed in the key event, false otherwise.
 *
 * Parameters:
 *
 *  XKeyEvent * - pointer to XKeyEvent
 *
 * Returns:
 *  
 *  bool
 */
bool is_shift_pressed(XKeyEvent *);

/*
 * Function: is_mod1_pressed
 *
 * Returns true if Mod1 is pressed in the key event, false otherwise.
 *
 * Parameters:
 *
 *  XKeyEvent * - pointer to XKeyEvent
 *
 * Returns:
 *
 *  bool
 */
bool is_mod1_pressed(XKeyEvent *);

/*
 * Function: is_mod2_pressed
 *
 * Returns true if Mod2 is pressed in the key event, false otherwise.
 *
 * Parameters:
 *
 *  XKeyEvent * - pointer to XKeyEvent
 *
 * Returns:
 *
 *  bool
 */
bool is_mod2_pressed(XKeyEvent *);

/*
 * Function: is_mod3_pressed
 * 
 * Returns true if Mod3 is pressed in the key event, false otherwise.
 *
 * Parameters:
 *
 *  XKeyEvent * - pointer to XKeyEvent
 *
 * Returns:
 *
 *  bool
 */
bool is_mod3_pressed(XKeyEvent *);

/*
 * Function: is_mod4_pressed
 *
 * Returns true if Mod4 is pressed in the key event, false otherwise.
 *
 * Parameters:
 *
 *  XKeyEvent * - pointer to XKeyEvent
 *
 * Returns:
 *
 *  bool
 */
bool is_mod4_pressed(XKeyEvent *);

/*
 * Function: is_ks_pressed
 *
 * Returns true if the key shortcut is being pressed in the key event,
 * false otherwise.
 *
 * Parameters:
 *
 *  const char * - the key shortcut
 *  XKeyEvent * - pointer to XKeyEvent
 *
 * Returns:
 *
 *  bool
 *
 */
bool is_ks_pressed(const char *, XKeyEvent *);

#endif

