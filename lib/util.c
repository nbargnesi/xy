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

#include "xy.h"
#include "util.h"
#include "types.h"
#include "constants.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
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

char ** parse_command(const char *cmd) {
    char *input = strdup(cmd);
    int token_ct = 0;
    char **tokens = malloc(sizeof(char *));
    char *token = strtok(input, " ");

    while (token != NULL) {
        tokens = realloc(tokens, sizeof(char *) * (token_ct + 1));
        tokens[token_ct] = strdup(token);
        token_ct += 1;
        token = strtok(NULL, " ");
    }
    tokens = realloc(tokens, sizeof(char *) * (token_ct + 1));
    tokens[token_ct] = NULL;
    free(input);
    return tokens;
}

pid_t exec(const char *cmd) {
    char **argv = parse_command(cmd);
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
        sigaction(SIGCHLD, _sighndlr, NULL);
        execvp(*argv, argv);
    }
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

