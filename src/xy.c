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
#include "ipc.h"

#define log log4c_category_log
#define FUNCTION_TRACE log_trace(global_log, __FUNCTION__);

void main_loop() {

    /*
    int s = DefaultScreen(global_display);
    Display *d = global_display;
    Window w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 200, 200, 1,
            BlackPixel(d, s), WhitePixel(d, s));
    XSelectInput(d, w, ExposureMask | KeyPressMask);
    XMapWindow(d, w);
    XSync(global_display, False);
    XEvent e;
    */

    Display *d = global_display;
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
            log_debug(global_log, "servicing X");
            XNextEvent(d, &e);
        } else if (FD_ISSET(global_ipc_fd, &set)) {
            log_debug(global_log, "servicing IPC");
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
    FUNCTION_TRACE
    transition(STARTING_UP);
    return 0;
}

// TODO push into configuration.c
void configure(CONFIG *cfg) {
    FUNCTION_TRACE

    // Should we skip the window manager check?
    const char *wmcheck = get_config_value(cfg, CFG_SKIP_WINDOW_MGR_CHECK);
    if (!wmcheck || strcmp(wmcheck, "true") != 0) {
        if (is_window_manager_running(global_display)) {
            log_fatal(global_log, WINDOW_MGR_RUNNING);
        }
    } else
        log_info(global_log, SKIP_WINDOW_MGR_CHECK_MSG);

    // Should we change the window manager's name?
    char *wmname = get_config_value(cfg, CFG_WINDOW_MGR_NAME);
    if (wmname) {
        log_info(global_log, CHANGE_WINDOW_MGR_NAME_MSG);
        change_name(global_display, wmname);
    }
}

void ipc_quit() {
    broadcast_send(SHUTTING_DOWN_MSG);
    transition(SHUTTING_DOWN);
}

void ipc_ping() {
    broadcast_send(PONG_MSG);
}

