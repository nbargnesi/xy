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
    int max_sd, rc;
    fd_set set;

    FD_ZERO(&set);
    FD_SET(global_ipc_fd, &set);
    FD_SET(global_x_fd, &set);
    if (global_x_fd > global_ipc_fd)
        max_sd = global_x_fd + 1;
    else
        max_sd = global_ipc_fd + 1;

    for (;;) {
        rc = select(max_sd, &set, NULL, NULL, NULL);
        if (unlikely(rc < 0)) {
            perror("select()");
            DIE
        }
        if (FD_ISSET(global_x_fd, &set)) {
            log_debug(global_log, "servicing X");
            // TODO service X connection
        } else if (FD_ISSET(global_ipc_fd, &set)) {
            log_debug(global_log, "servicing IPC");
            // TODO service IPC connection
        }
    }
}

int main(int argc, char **argv) {
    FUNCTION_TRACE
    transition(STARTING_UP);

    broadcast_send(SHUTTING_DOWN_MSG);
    transition(SHUTTING_DOWN);
    return 0;
}

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

