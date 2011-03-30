#include "state.h"
#include "startup.h"

void transition(const State state) {
    switch (state) {
        case INITIALIZING_MODULES:
            module_init();
            break;
    }
}

