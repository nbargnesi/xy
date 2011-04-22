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

#include "util.h"
#include "types.h"

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

