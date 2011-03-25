#include "xyxlib.h"

CLIENTS_LIST * cl_init() {
    CLIENTS_LIST *ret = malloc(sizeof(CLIENTS_LIST));
    memset(ret, 0, sizeof(CLIENTS_LIST));
    ret->root = malloc(sizeof(Window));
    ret->parent = malloc(sizeof(Window));
    return ret;
}

CLIENTS_LIST * get_clients(Display *d, Window w) {
    CLIENTS_LIST *ret = cl_init();

    Window *root = ret->root;
    Window *parent = ret->parent;

    Status s = XQueryTree(d, w, root, parent, &ret->children, &ret->num_children);
    if (!s) {
        free(ret);
        return NULL;
    }
    return ret;
}

XWindowAttributes * get_attributes(Display *d, Window w) {
    XWindowAttributes *ret = malloc(sizeof(XWindowAttributes));
    Status s = XGetWindowAttributes(d, w, ret);
    if (!s) {
        free(ret);
        return NULL;
    }
    return ret;
}

void clients_list_free(CLIENTS_LIST *cl) {
    if (!cl) return;
    free(cl->root);
    free(cl->parent);
    XFree(cl->children);
    free(cl);
}

