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

#include "xy.h"
#include "util.h"
#include "types.h"
#include "constants.h"

#include <ctype.h>
#include <execinfo.h>
#include <sys/prctl.h>

void dump_stack(int num_frames) {
    void *ptrs[num_frames];
    size_t count = backtrace(ptrs, num_frames);
    char **funcs = backtrace_symbols(ptrs, num_frames);
    for (uint i = 0; i < count; i++)
        fprintf(stderr, "%s\n", funcs[i]);
    free(funcs);
}

void parse_command(char *cmd, char **argv) {
    while (*cmd != '\0') {
        while (*cmd == ' ') *cmd++ = '\0';
        *argv++ = cmd;
        while (*cmd != '\0' && *cmd != ' ') cmd++;
    }
    *argv = '\0';
}

pid_t exec(const char *cmd) {
    char * cmd_dup = strdup(cmd);
    char *argv[64];
    parse_command(cmd_dup, argv);
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        DIE;
    } else if (pid == 0) {
        if (globals) {
            close(globals->x_fd);
            close(globals->ipc_fd);
            close(globals->in_fd);
        }
        execvp(*argv, argv);
    }
    free(cmd_dup);
    return pid;
}

void change_name(Display *d, const char *name) {
    if (!name) return;
    Window root = DefaultRootWindow(d);
    Atom wmcheck = XInternAtom(d, "_NET_SUPPORTING_WM_CHECK", False);
    Atom wmname = XInternAtom(d, "_NET_WM_NAME", False);
    Atom utfstring = XInternAtom(d, "UTF8_STRING", False);

    uchar *data = (uchar *) &root;
    XChangeProperty(d, root, wmcheck, XA_WINDOW, 32, PropModeReplace, data, 1);
    data = (uchar *) name;
    int len = strlen(name);
    XChangeProperty(d, root, wmname, utfstring, 8, PropModeReplace, data, len);
}

void set_process_name(const char *name) {
    if (!name) return;
    char buffer[MAX_PROC_NAME_LEN + 1];
    strncpy(buffer, name, MAX_PROC_NAME_LEN);
    // Must be null terminated
    buffer[MAX_PROC_NAME_LEN] = 0;
    if (prctl(PR_SET_NAME, (unsigned long) &buffer) == -1) {
        perror(__func__);
    }
}

static char * left_trim(char *str) {
    while (isspace(*str)) str++;
    return str;
}

static char * right_trim(char *str) {
    char *c = str + strlen(str);
    while (isspace(*--c));
    *(c + 1) = '\0';
    return str;
}

char * trim(char *str) {
    return right_trim(left_trim(str));
}

bool streq(const char *s1, const char *s2) {
    if (s1 == NULL && s2 == NULL) return true;
    else if (s1 == NULL || s2 == NULL) return false;
    if (strcmp(s1, s2) == 0) return true;
    return false;
}

void restart(const char *cmd) {
    execlp(cmd, cmd, NULL, NULL, NULL);
}

char * rc_path() {
    char *home = getenv("HOME");
    if (!home) DIE;

    const int bufsize = strlen(home) + strlen(XY_CONFIG) + 2;
    char *rcpath = malloc(bufsize);
    memset(rcpath, 0, bufsize);
    strcat(rcpath, home);
    strcat(rcpath, "/");
    strcat(rcpath, XY_CONFIG);
    return rcpath;
}

