#include "xinerama.h"
#include <stdlib.h>
#include <string.h>

bool is_xinerama_active(Display *d) {
    if (XineramaIsActive(d) == True)
        return true;
    return false;
}

int get_xinerama_screen_count(Display *d) {
    int i;
    XineramaScreenInfo *xsi = XineramaQueryScreens(d, &i);
    XFree(xsi);
    return i;
}

XineramaScreenInfo * get_xinerama_screen(Display *d, int i) {
    XineramaScreenInfo *ret = malloc(sizeof(XineramaScreenInfo));
    memset(ret, 0, sizeof(XineramaScreenInfo));
    XineramaScreenInfo *xsi = XineramaQueryScreens(d, &i);
    ret->screen_number = xsi->screen_number;
    ret->x_org = xsi->x_org;
    ret->y_org = xsi->y_org;
    ret->width = xsi->width;
    ret->height = xsi->height;
    XFree(xsi);
    return ret;
}

XineramaScreenInfo * get_xinerama_screens(Display *d, int *i) {
    return XineramaQueryScreens(d, i);
}

