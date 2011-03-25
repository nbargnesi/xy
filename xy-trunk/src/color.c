#include "color.h"

ulong get_color(const char *color, Display *d, Colormap colormap) {
    XColor xc;
    if (!XAllocNamedColor(d, colormap, color, &xc, &xc))
        return 0L;
    return xc.pixel;
}

