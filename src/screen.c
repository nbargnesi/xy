#include "screen.h"
#include <stdlib.h>
#include <stdio.h>

SCREEN * default_screen(Display *d) {
    const int default_screen = DefaultScreen(d);
    SCREEN *screen = malloc(sizeof(SCREEN));
    screen->number = default_screen;
    screen->root = RootWindow(d, default_screen);
    screen->width = DisplayWidth(d, default_screen);
    screen->height = DisplayHeight(d, default_screen);
    screen->depth = DefaultDepth(d, default_screen);
    screen->visual = DefaultVisual(d, default_screen);
    screen->colormap = DefaultColormap(d, default_screen);
    return screen;
}

