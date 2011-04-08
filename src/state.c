#include "state.h"
#include "lifecycle.h"

void transition(const State state) {
    switch (state) {
        case STARTING_UP:
            module_init();
            directory_init();
            setup();
            break;
        case STARTED:
            break;
        case SHUTTING_DOWN:
            shutdown();
            break;
    }
}

