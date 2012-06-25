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

bool is_control_pressed(XKeyEvent *xke) {
    if (!xke) return false;
    return (xke->state & ControlMask) == ControlMask;
}

bool is_shift_pressed(XKeyEvent *xke) {
    if (!xke) return false;
    return (xke->state & ShiftMask) == ShiftMask;
}

bool is_mod1_pressed(XKeyEvent *xke) {
    if (!xke) return false;
    return (xke->state & Mod1Mask) == Mod1Mask;
}

bool is_mod2_pressed(XKeyEvent *xke) {
    if (!xke) return false;
    return (xke->state & Mod2Mask) == Mod2Mask;
}

bool is_mod3_pressed(XKeyEvent *xke) {
    if (!xke) return false;
    return (xke->state & Mod3Mask) == Mod3Mask;
}

bool is_mod4_pressed(XKeyEvent *xke) {
    if (!xke) return false;
    return (xke->state & Mod4Mask) == Mod4Mask;
}

bool is_ks_pressed(const char *ks, XKeyEvent *xke) {
    if (!ks) {
        return false;
    }
    KeySym keysym = XkbKeycodeToKeysym(globals->dpy, xke->keycode, 0, 0);
    const char *keystr = XKeysymToString(keysym);
    char *ks2 = strdup(ks);
    bool shft = is_shift_pressed(xke);
    bool ctrl = is_control_pressed(xke);
    bool mod1 = is_mod1_pressed(xke);
    bool mod2 = is_mod2_pressed(xke);
    bool mod3 = is_mod3_pressed(xke);
    bool mod4 = is_mod4_pressed(xke);
    char *token = strtok(ks2, " ");
    while (token) {
        if (streq(token, SHIFT_KS) && !shft) goto return_false;
        else if (streq(token, CTRL_KS) && !ctrl) goto return_false;
        else if (streq(token, MOD1_KS) && !mod1) goto return_false;
        else if (streq(token, MOD2_KS) && !mod2) goto return_false;
        else if (streq(token, MOD3_KS) && !mod3) goto return_false;
        else if (streq(token, MOD4_KS) && !mod4) goto return_false;

        if (streq(token, keystr)) {
            goto return_true;
        }

        token = strtok(NULL, " ");
    }

return_false:
    free(ks2);
    return false;

return_true:
    free(ks2);
    return true;
}

