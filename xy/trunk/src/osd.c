#include "osd.h"
#include <xosd.h>

static xosd *osd;

void osd_init() {
    osd = xosd_create(1);
}

