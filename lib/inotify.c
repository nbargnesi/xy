/*
 * Copyright (c) 2011-2012 Nick Bargnesi
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

#include "inotify.h"
#include "lifecycle.h"
#include "util.h"
#include "xy.h"
#include <sys/ioctl.h>

static int inotify_setup();

void xy_inotify_init() {
    in_fd = inotify_setup();
    globals->in_fd = in_fd;
    register_shutdown_hook("inotify", xy_inotify_terminate);
}

void xy_inotify_reinit() {
    close(globals->in_fd);
    in_fd = inotify_setup();
    globals->in_fd = in_fd;
}

void xy_inotify_terminate() {
    close(globals->in_fd);
}

bool xy_inotify_read() {
    int nbytes;
    int ctlval = ioctl(in_fd, FIONREAD, &nbytes);
    if (ctlval < 0) return false;
    char *buf = (char *) malloc(nbytes);
    int rval = read(in_fd, buf, nbytes);
    uint offset = 0;
    bool success = true;
    while (rval != 0) {
        struct inotify_event *ev = (struct inotify_event *) &buf[offset];
        int evSize, sSize = sizeof(struct inotify_event);
        evSize = sSize;
        if (ev->len != 0) evSize += ev->len;
        if (ev->mask == IN_IGNORED) success = false;
        rval -= evSize;
        offset += evSize;
    }
    free(buf);
    return success;
}

static int inotify_setup() {
    int in_fd = inotify_init();
    if (in_fd == -1) {
        perror("inotify_init()");
        return -1;
    }
    char *rcpath = rc_path();
    // TODO check rcpath existence, fail if not there (indicates problem with
    // module load ordering)
    uint32_t mask = IN_CLOSE_WRITE | IN_MODIFY;
    int wd = inotify_add_watch(in_fd, rcpath, mask);
    free(rcpath);
    if (wd == -1) {
        perror("inotify_add_watch()");
        close(globals->in_fd);
        return -1;
    }
    return in_fd;
}

