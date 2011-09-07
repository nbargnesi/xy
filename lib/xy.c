/*
 * This file is part of xy.
 *
 * XY is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XY is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with xy.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "xy.h"

#define log log4c_category_log
#define FUNCTION_TRACE log_trace(global_log, __FUNCTION__);

void main_loop() {

    ssize_t result;
    Display *d = global_display;
    /*
    int s = DefaultScreen(global_display);
    Window root = RootWindow(d, s);
    Window w = XCreateSimpleWindow(d, root, 10, 10, 200, 200, 1,
            BlackPixel(d, s), WhitePixel(d, s));
    XSelectInput(d, w, 0x1ffff);
    XMapWindow(d, w);
    XSync(global_display, False);

    XSetWindowAttributes wa;
    wa.event_mask = SubstructureRedirectMask|SubstructureNotifyMask|StructureNotifyMask|PropertyChangeMask;
    XChangeWindowAttributes(d, root, CWEventMask, &wa);
    XSelectInput(d, root, wa.event_mask | 0x1ffff);
    */

    XEvent e;
    int max_sd, rc;
    fd_set set;
    char *ipc_buffer;

    if (global_x_fd > global_ipc_fd)
        max_sd = global_x_fd + 1;
    else
        max_sd = global_ipc_fd + 1;

    log_info(global_log, STARTED_MSG);
    broadcast_send(STARTED_MSG);
    for (;;) {
        FD_ZERO(&set);
        FD_SET(global_ipc_fd, &set);
        FD_SET(global_x_fd, &set);
        rc = select(max_sd, &set, NULL, NULL, NULL);
        if (unlikely(rc < 0)) {
            perror("select()");
            DIE;
        }

        if (FD_ISSET(global_x_fd, &set)) {
            XNextEvent(d, &e);
            event_handler(&e);
        } else if (FD_ISSET(global_ipc_fd, &set)) {
            ipc_buffer = malloc(MSG_LEN);
            memset(ipc_buffer, 0, MSG_LEN);
            result = read(global_ipc_fd, ipc_buffer, MSG_LEN);
            process_ipc_buffer(ipc_buffer);
            free(ipc_buffer);
        }
    }
}

void ipc_quit() {
    transition(SHUTTING_DOWN);
}

void ipc_ping() {
    broadcast_send(PONG_MSG);
}

void key_pressed(XKeyEvent *ev) {
    KeySym keysym = XKeycodeToKeysym(global_display, ev->keycode, 0);
    if (is_ks_pressed(get_menu_shortcut(), ev)) {
        fprintf(stderr, "menu shortcut pressed\n");
    } else if (is_ks_pressed(get_terminal_shortcut(), ev)) {
        fprintf(stderr, "terminal shortcut pressed\n");
        exec(get_terminal_command());
    } else if (is_ks_pressed(get_quit_shortcut(), ev)) {
        transition(SHUTTING_DOWN);
    }
}

void key_released(XKeyEvent *ev) {
}

void configure_request(XConfigureRequestEvent *ev) {
}

void configure_notify(XConfigureEvent *ev) {
}

void destroy_notify(XDestroyWindowEvent *ev) {
}

void enter_notify(XCrossingEvent *ev) {
}

void expose(XExposeEvent *ev) {
}

void focus_event(XFocusChangeEvent *ev) {
}

void mapping_notify(XMappingEvent *ev) {
}

void map_request(XMapRequestEvent *ev) {
}

void property_notify(XPropertyEvent *ev) {
}

void unmap_notify(XUnmapEvent *ev) {
}
