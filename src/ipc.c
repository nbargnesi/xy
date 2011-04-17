#include "ipc.h"
#include "xy.h"

static struct sockaddr_un *ipc_sckt;
static const char *ipc_path;

bool ipc_init(const char *path) {
    ipc_path = strdup(path);
    int len;
    ipc_sckt = malloc(sizeof(struct sockaddr_un));
    ipc_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (ipc_fd < 0) {
        perror("opening IPC socket");
        return false;
    }
    
    ipc_sckt->sun_family = AF_UNIX;
    strcpy(ipc_sckt->sun_path, ipc_path);

    if (bind(ipc_fd, (struct sockaddr *) ipc_sckt, sizeof(struct sockaddr_un))) {
        perror("binding name to dgram socket");
        return false;
    }

    return true;
}

void ipc_terminate() {
    unlink(ipc_path);
}

