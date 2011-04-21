#include "xy.h"

#define log log4c_category_log
#define FUNCTION_TRACE log_trace(xylog, __FUNCTION__);

void main_loop() {
    int max_sd, rc;
    fd_set set;

    FD_ZERO(&set);
    max_sd = ipc_fd;
    FD_SET(ipc_fd, &set);

    rc = select(max_sd + 1, &set, NULL, NULL, NULL);

    if (rc < 0) {
        perror("select()");
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
            log_fatal(xylog, WINDOW_MGR_RUNNING);
        }
    } else
        log_info(xylog, SKIP_WINDOW_MGR_CHECK_MSG);

    // Should we change the window manager's name?
    char *wmname = get_config_value(cfg, CFG_WINDOW_MGR_NAME);
    if (wmname) {
        log_info(xylog, CHANGE_WINDOW_MGR_NAME_MSG);
        change_name(global_display, wmname);
    }
}

