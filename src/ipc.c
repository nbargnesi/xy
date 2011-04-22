#include "ipc.h"
#include "xy.h"

static struct sockaddr_un *ipc_sckt;
static const char *ipc_path;

bool ipc_init(const char *path) {
    ipc_path = strdup(path);
    unlink(ipc_path);
    ipc_sckt = malloc(sizeof(struct sockaddr_un));
    global_ipc_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (global_ipc_fd < 0) {
        perror("opening IPC");
        return false;
    }
    
    ipc_sckt->sun_family = AF_UNIX;
    strcpy(ipc_sckt->sun_path, ipc_path);

    ssize_t size = sizeof(struct sockaddr_un);
    if (bind(global_ipc_fd, (struct sockaddr *) ipc_sckt, size)) {
        perror("binding to IPC");
        return false;
    }

    return true;
}

void ipc_terminate() {
    unlink(ipc_path);
}

