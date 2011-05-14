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
    XKeyEvent *keyev;
    XConfigureRequestEvent *conreqev;
    XConfigureEvent *confev;
    XDestroyWindowEvent *destroyev;
    XCrossingEvent *crossev;
    XExposeEvent *exposeev;
    XFocusChangeEvent *focchgev;
    XMappingEvent *mapev;
    XMapRequestEvent *mapreqev;
    XPropertyEvent *propev;
    XUnmapEvent *unmapev;

    switch (xev->type) {
        case KeyPress:
            keyev = &xev->xkey;
            key_pressed(keyev);
            break;
        case KeyRelease:
            keyev = &xev->xkey;
            key_released(keyev);
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
            crossev = &xev->xcrossing;
            enter_notify(crossev);
            break;
        case LeaveNotify:
            fprintf(stderr, "leave notify\n");
            break;
        case FocusIn:
            focchgev = &xev->xfocus;
            focus_event(focchgev); 
            break;
        case FocusOut:
            fprintf(stderr, "focus-out\n");
            break;
        case KeymapNotify:
            fprintf(stderr, "keymap notify\n");
            break;
        case Expose:
            exposeev = &xev->xexpose;
            expose(exposeev);
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
            unmapev = &xev->xunmap;
            unmap_notify(unmapev);
            break;
        case MapNotify:
            mapev = &xev->xmapping;
            mapping_notify(mapev);
            break;
        case MapRequest:
            mapreqev = &xev->xmaprequest;
            map_request(mapreqev);
            break;
        case ReparentNotify:
            fprintf(stderr, "reparent notify\n");
            break;
        case ConfigureNotify:
            confev = &xev->xconfigure;
            configure_notify(confev);
            break;
        case ConfigureRequest:
            conreqev = &xev->xconfigurerequest;
            configure_request(conreqev);
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
            propev = &xev->xproperty;
            property_notify(propev);
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

