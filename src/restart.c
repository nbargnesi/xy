#include "restart.h"

void restart(const char *cmd) {
    execlp(cmd, cmd, NULL, NULL, NULL);
}

