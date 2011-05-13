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

    Display *d = global_display;
    int s = DefaultScreen(global_display);
    Window root = RootWindow(d, s);
    Window w = XCreateSimpleWindow(d, root, 10, 10, 200, 200, 1,
            BlackPixel(d, s), WhitePixel(d, s));
    XSelectInput(d, w, 0x1ffff);
    XMapWindow(d, w);
    XSync(global_display, False);

    /*
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

    for (;;) {
        FD_ZERO(&set);
        FD_SET(global_ipc_fd, &set);
        FD_SET(global_x_fd, &set);
        rc = select(max_sd, &set, NULL, NULL, NULL);
        if (unlikely(rc < 0)) {
            perror("select()");
            DIE
        }

        if (FD_ISSET(global_x_fd, &set)) {
            XNextEvent(d, &e);
            event_handler(&e);
        } else if (FD_ISSET(global_ipc_fd, &set)) {
            ipc_buffer = malloc(MSG_LEN);
            memset(ipc_buffer, 0, MSG_LEN);
            read(global_ipc_fd, ipc_buffer, MSG_LEN);
            process_ipc_buffer(ipc_buffer);
            free(ipc_buffer);
        }
    }
}

// TODO break out into main.c
int main(int argc, char **argv) {
    transition(STARTING_UP);
    return 0;
}

void ipc_quit() {
    transition(SHUTTING_DOWN);
}

void ipc_ping() {
    broadcast_send(PONG_MSG);
}

void key_pressed(XEvent *ev) {
    XKeyEvent *xke = &ev->xkey;
    KeySym keysym = XKeycodeToKeysym(global_display, xke->keycode, 0);
    if (is_ks_pressed(get_menu_shortcut(), xke)) {
        fprintf(stderr, "menu shortcut pressed\n");
    } else if (is_ks_pressed(get_terminal_shortcut(), xke)) {
        fprintf(stderr, "terminal shortcut pressed\n");
        exec(get_terminal_command());
    } else if (is_ks_pressed(get_quit_shortcut(), xke)) {
        transition(SHUTTING_DOWN);
    }
}

void key_released(XEvent *ev) {
}

