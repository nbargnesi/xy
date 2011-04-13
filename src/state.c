#include "state.h"
#include "lifecycle.h"

void transition(const State state) {
    switch (state) {
        case STARTING_UP:
            startup();
            break;
        case STARTED:
            started();
            break;
        case SHUTTING_DOWN:
            shutting_down();
            break;
        case SHUTDOWN:
            shutdown();
            break;
    }
}

