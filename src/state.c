#include "state.h"
#include "lifecycle.h"

void transition(const State state) {
    switch (state) {
        case STARTING_UP:
            xy_startup();
            break;
        case STARTED:
            xy_started();
            break;
        case SHUTTING_DOWN:
            xy_shutting_down();
            break;
        case SHUTDOWN:
            xy_shutdown();
            break;
    }
}

