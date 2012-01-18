/*
 * Copyright 2011-2012 Nick Bargnesi <nick@den-4.com>. All rights reserved.
 *
 * This file is part of xy.
 *
 * XY is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XY is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with xy.  If not, see <http://www.gnu.org/licenses/>.
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

void process_ipc_buffer(const char *buffer) {
    COMMAND cmd = convert_command_str(buffer);
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
            xy_restart();
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
    close(global_ipc_fd);
    unlink(ipc_path);
}

