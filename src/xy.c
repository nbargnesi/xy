#include "xy.h"

#define log log4c_category_log
#define FUNCTION_TRACE log_trace(xylog, __FUNCTION__);

int main(int argc, char **argv) {
    transition(STARTING_UP);
    FUNCTION_TRACE
    configure(global_cfg);

    SCREEN *screen = default_screen(global_display);
    CLIENTS_LIST *clients = get_clients(global_display, screen->root);
    for (int i = 0; i < clients->num_children; i++) {
        Window w = clients->children[i];
        XWindowAttributes *attrs = get_attributes(global_display, w);
        if (attrs->map_state == IsViewable)
        fprintf(stderr, "%d: x: %d, y: %d, w: %d, h: %d\n",
                w, attrs->x, attrs->y, attrs->width, attrs->height);
        free(attrs);
    }
    if (clients) clients_list_free(clients);
    else log_fatal(xylog, FAILED_TO_GET_CLIENTS);

    if (screen) free(screen);

    broadcast_send("shutting down");
    transition(SHUTTING_DOWN);
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

