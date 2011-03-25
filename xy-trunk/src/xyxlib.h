#ifndef XYXLIB_H_
#define XYXLIB_H_

#include "core.h"

typedef struct {
    Window *root;
    Window *parent;
    Window *children;
    uint num_children;
} CLIENTS_LIST;

CLIENTS_LIST * get_clients(Display *, Window);
XWindowAttributes * get_attributes(Display *, Window);
void clients_list_free(CLIENTS_LIST *);

#endif
