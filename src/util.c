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

#include "xy.h"
#include "util.h"
#include "types.h"
#include "constants.h"

#include <ctype.h>
#include <execinfo.h>

void dump_stack(int num_frames) {
    void *ptrs[num_frames];
    size_t count = backtrace(ptrs, num_frames);
    char **funcs = backtrace_symbols(ptrs, num_frames);
    for (uint i = 0; i < count; i++)
        fprintf(stderr, "%s\n", funcs[i]);
    free(funcs);
}

void change_name(Display *d, const char *name) {
    if (!name) return;
    Window root = DefaultRootWindow(d);
    Atom wmcheck = XInternAtom(d, "_NET_SUPPORTING_WM_CHECK", False);
    Atom wmname = XInternAtom(d, "_NET_WM_NAME", False);
    Atom utfstring = XInternAtom(d, "UTF8_STRING", False);

    uchar *data = (uchar *) &root;
    XChangeProperty(d, root, wmcheck, XA_WINDOW, 32, PropModeReplace, data, 1);
    data = (uchar *) name;
    int len = strlen(name);
    XChangeProperty(d, root, wmname, utfstring, 8, PropModeReplace, data, len);
}

static char * left_trim(char *str) {
    while (isspace(*str)) str++;
    return str;
}

static char * right_trim(char *str) {
    char *c = str + strlen(str);
    while (isspace(*--c));
    *(c + 1) = '\0';
    return str;
}

char * trim(char *str) {
    return right_trim(left_trim(str));
}

bool streq(const char *s1, const char *s2) {
    if (s1 == NULL && s2 == NULL) return true;
    else if (s1 == NULL || s2 == NULL) return false;
    if (strcmp(s1, s2) == 0) return true;
    return false;
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
    KeySym keysym = XKeycodeToKeysym(global_display, xke->keycode, 0);
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

