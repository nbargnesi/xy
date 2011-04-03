#include "xy.h"

#define log log4c_category_log
#define FUNCTION_TRACE log_trace(xylog, __FUNCTION__);

Display *d;
CONFIG *cfg;
log4c_category_t *xylog;

int main(int argc, char **argv) {
    transition(INITIALIZING_MODULES);

    xylog = get_logger("xy");
    FUNCTION_TRACE

    d = open_display();

    cfg = get_configuration();
    configure(cfg);

    SCREEN *screen = default_screen(d);
    CLIENTS_LIST *clients = get_clients(d, screen->root);
    for (int i = 0; i < clients->num_children; i++) {
        Window w = clients->children[i];
        XWindowAttributes *attrs = get_attributes(d, w);
        if (attrs->map_state == IsViewable)
        fprintf(stderr, "%d: x: %d, y: %d, w: %d, h: %d\n",
                w, attrs->x, attrs->y, attrs->width, attrs->height);
    }
    if (clients) clients_list_free(clients);
    else log_fatal(xylog, "failed to get clients");

    if (cfg) free_config(cfg);
    if (screen) free(screen);
    close_display(d);

    transition(TERMINATING_MODULES);
}

void configure(CONFIG *cfg) {
    FUNCTION_TRACE

    // Should we skip the window manager check?
    const char *wmcheck = get_config_value(cfg, CFG_NAME_SKIP_WINDOW_MGR_CHECK);
    if (!wmcheck || strcmp(wmcheck, "true") != 0) {
        if (is_window_manager_running(d)) {
            log_fatal(xylog, WINDOW_MGR_RUNNING);
        }
    } else
        log_info(xylog, SKIP_WINDOW_MGR_CHECK_MSG);

    // Should we change the window manager's name?
    char *wmname = get_config_value(cfg, CFG_NAME_WINDOW_MANAGER_NAME);
    if (wmname) {
        log_info(xylog, CHANGE_WINDOW_MGR_NAME_MSG);
        change_name(d, wmname);
    }
}

CONFIG * get_configuration() {
    FUNCTION_TRACE

    char *home = getenv("HOME");
    if (!home)
        goto default_configuration;

    struct stat *st = malloc(sizeof(struct stat));;
    char *path = strcat(home, CONFIG_FILE);

    if (stat(path, st) != 0) {
        log(xylog, INFO, "no configuration %s", path);
        goto default_configuration;
    }
    log_info(xylog, CONFIGURATION_READ_MSG);

    cfg = get_config(path);
    free(st);
    return cfg;

default_configuration:
    log_warn(xylog, USING_DEFAULT_CONFIGURATION_MSG);
    cfg = empty_config();
    return cfg;
}

