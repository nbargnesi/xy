#ifndef SERVER_H_
#define SERVER_H_

#include "types.h"
#include "core.h"

typedef struct {
    Window root;
    uint number;
    uint width;
    uint height;
    uint depth;
    Visual *visual;
    Colormap colormap;
} SCREEN;

SCREEN * default_screen(Display *);

#endif

