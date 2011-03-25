#ifndef OSD_H_
#define OSD_H_

#include "core.h"

enum OSD_Vert_Align {
    TOP,
    MIDDLE,
    BOTTOM
};

enum OSD_Horiz_Align {
    LEFT,
    CENTER,
    RIGHT
};

void osd_init();

#endif

