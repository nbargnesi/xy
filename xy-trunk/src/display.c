#include "display.h"

Display * open_display() {
    return XOpenDisplay(NULL);
}
void close_display(Display *d) {
    XCloseDisplay(d);
}

