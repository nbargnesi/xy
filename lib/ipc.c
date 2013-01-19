/*
 * Copyright (c) 2011-2013 Nick Bargnesi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "ipc.h"
#include "xy.h"
#include "util.h"

static struct sockaddr_un *ipc_sckt;
static const char *ipc_path;

bool ipc_init() {
    char *home = getenv("HOME");
    if (!home) DIE;

    const int bufsize = strlen(home) + strlen(IPC_SOCKET_PATH) + 2;
    char *path = malloc(bufsize);
    memset(path, 0, bufsize);
    strcat(path, home);
    strcat(path, "/");
    strcat(path, IPC_SOCKET_PATH);

    ipc_path = strdup(path);
    free(path);
    unlink(ipc_path);
    ipc_sckt = malloc(sizeof(struct sockaddr_un));
    globals->ipc_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (globals->ipc_fd < 0) {
        perror("opening IPC");
        return false;
    }
    
    ipc_sckt->sun_family = AF_UNIX;
    strcpy(ipc_sckt->sun_path, ipc_path);

    ssize_t size = sizeof(struct sockaddr_un);
    if (bind(globals->ipc_fd, (struct sockaddr *) ipc_sckt, size)) {
        perror("binding to IPC");
        return false;
    }

    register_shutdown_hook("ipc", ipc_terminate);
    return true;
}

void process_ipc_buffer(char *buffer) {
    COMMAND cmd = convert_command_str(buffer);
    free(buffer);
    switch (cmd) {
        case PING:
            ipc_ping();
            break;
        case NO_OP:
            break;
        case QUIT:
            ipc_quit();
            break;
        case RESTART:
            transition(STATE_RESTARTING);
            break;
    }
}

COMMAND convert_command_str(const char *cmd) {
    if (streq("QUIT", cmd) || streq("QUIT", cmd))
        return QUIT;
    else if (streq("PING", cmd) || streq("ping", cmd))
        return PING;
    else if (streq("RESTART", cmd) || streq("restart", cmd))
        return RESTART;
    return NO_OP;
}

void ipc_terminate() {
    close(globals->ipc_fd);
    unlink(ipc_path);
}

