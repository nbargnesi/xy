#ifndef XINERAMA_H_
#define XINERAMA_H_

#include "core.h"

bool is_xinerama_active(Display *);
int get_xinerama_screen_count(Display *);
XineramaScreenInfo * get_xinerama_screen(Display *, int);
XineramaScreenInfo * get_xinerama_screens(Display *, int *);

#endif

