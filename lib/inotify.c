/*
 * Copyright 2012 Nick Bargnesi <nick@den-4.com>. All rights reserved.
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

#include "inotify.h"
#include "util.h"
#include <sys/ioctl.h>

int xy_inotify_init() {
    xy_in_fd = inotify_init();
    if (xy_in_fd == -1) {
        perror("inotify_init()");
        return -1;
    }

    char *rcpath = rc_path();
    int wd = inotify_add_watch(xy_in_fd, rcpath, IN_CLOSE_WRITE | IN_MODIFY);
    free(rcpath);
    if (wd == -1) {
        perror("inotify_add_watch()");
        close(xy_in_fd);
        return -1;
    }

    return xy_in_fd;
}

void xy_inotify_read() {
    int nbytes;
    int ctlval = ioctl(xy_in_fd, FIONREAD, &nbytes);
    if (ctlval < 0) return;
    char *buf = (char *) malloc(nbytes);
    int rval = read(xy_in_fd, buf, nbytes);
    free(buf);
}

