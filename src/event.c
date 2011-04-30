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

#include "event.h"
#include "xy.h"

void event_handler(XEvent *xev) {
    switch (xev->type) {
        case KeyPress:
            key_pressed(xev);
            break;
        case KeyRelease:
            key_released(xev);
            break;
        case ButtonPress:
            fprintf(stderr, "button press event\n");
            break;
        case ButtonRelease:
            fprintf(stderr, "button release event\n");
            break;
        case MotionNotify:
            fprintf(stderr, "motion notify event\n");
            break;
        case EnterNotify:
            fprintf(stderr, "enter notify\n");
            break;
        case LeaveNotify:
            fprintf(stderr, "leave notify\n");
            break;
        case FocusIn:
            fprintf(stderr, "focus-in\n");
            break;
        case FocusOut:
            fprintf(stderr, "focus-out\n");
            break;
        case KeymapNotify:
            fprintf(stderr, "keymap notify\n");
            break;
        case Expose:
            fprintf(stderr, "expose\n");
            break;
        case GraphicsExpose:
            fprintf(stderr, "graphics expose\n");
            break;
        case NoExpose:
            fprintf(stderr, "no expose\n");
            break;
        case VisibilityNotify:
            fprintf(stderr, "visibility notify\n");
            break;
        case CreateNotify:
            fprintf(stderr, "create notify\n");
            break;
        case DestroyNotify:
            fprintf(stderr, "destroy notify\n");
            break;
        case UnmapNotify:
            fprintf(stderr, "unmap notify\n");
            break;
        case MapNotify:
            fprintf(stderr, "map notify\n");
            break;
        case MapRequest:
            fprintf(stderr, "map request\n");
            break;
        case ReparentNotify:
            fprintf(stderr, "reparent notify\n");
            break;
        case ConfigureNotify:
            fprintf(stderr, "configure notify\n");
            break;
        case ConfigureRequest:
            fprintf(stderr, "configure request\n");
            break;
        case GravityNotify:
            fprintf(stderr, "gravity notify\n");
            break;
        case ResizeRequest:
            fprintf(stderr, "resize request\n");
            break;
        case CirculateNotify:
            fprintf(stderr, "circulate notify\n");
            break;
        case CirculateRequest:
            fprintf(stderr, "circulate request\n");
            break;
        case PropertyNotify:
            fprintf(stderr, "property notify\n");
            break;
        case SelectionClear:
            fprintf(stderr, "selection clear\n");
            break;
        case SelectionRequest:
            fprintf(stderr, "selection reqeuest\n");
            break;
        case SelectionNotify:
            fprintf(stderr, "selection notify\n");
            break;
        case ColormapNotify:
            fprintf(stderr, "colormap notify\n");
            break;
        case ClientMessage:
            fprintf(stderr, "client message\n");
            break;
        case MappingNotify:
            fprintf(stderr, "mapping notify\n");
            break;
        case GenericEvent:
            fprintf(stderr, "generic event\n");
            break;
    }
}

