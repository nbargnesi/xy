#include "xy.h"

#define log log4c_category_log
#define FUNCTION_TRACE log_trace(xylog, __FUNCTION__);

Display *d;
CONFIG *cfg;
log4c_category_t *xylog;

int main(int argc, char **argv) {
    if (!start_logging()) {
        fprintf(stderr, "xy: failed to start logging\n");
        exit(1);
    }
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

    stop_logging();

    if (cfg) free_config(cfg);
    if (screen) free(screen);
    close_display(d);
}

void configure(CONFIG *cfg) {
    FUNCTION_TRACE

    // Should we skip the window manager check?
    const char *wmcheck = get_config_value(cfg, "skip_window_manager_check");
    if (!wmcheck || strcmp(wmcheck, "true") != 0) {
        if (is_window_manager_running(d)) {
            log_fatal(xylog, "another window manager is already running");
        }
    } else
        log_info(xylog, "skipping window manager check per configuration");

    // Should we change the window manager's name?
    char *wmname = get_config_value(cfg, "window_manager_name");
    if (wmname) {
        log_info(xylog, "changing window manager name per configuration");
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
        log(xylog, DEBUG, "couldn't read %s", path);
        goto default_configuration;
    }
    log_info(xylog, "read configuration file");

    cfg = get_config(path);
    free(st);
    return cfg;

default_configuration:
    log_warn(xylog, "using default configuration");
    cfg = empty_config();
    return cfg;
}

