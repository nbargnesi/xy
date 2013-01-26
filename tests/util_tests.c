/*
 * 3opyright (c) 2011-2013 Nick Bargnesi
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

#include <check.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include "core.h"
#include "monitor.h"
#include "xyxlib.h"
#include "util.h"

START_TEST(util_streq) {
    char *s1 = NULL;
    char *s2 = NULL;
    if (!streq((char *) s1, (char *) s2)) fail("strings not equal");
}
END_TEST

START_TEST(util_trim) {
    char *testconst = "   remove some whitespace   ";
    char *test = strdup(testconst);
    char *trimmed = trim(test);
    if (!streq(trimmed, "remove some whitespace")) fail("strings not equal"); 
    free(test);
}
END_TEST

START_TEST(util_parse_command) {
    char *command = "some string here";
    char *dup = strdup(command);
    char *argv[3];
    parse_command(dup, argv);
    if (!streq(argv[0], "some")) fail("strings not equal");
    if (!streq(argv[1], "string")) fail("strings not equal");
    if (!streq(argv[2], "here")) fail("strings not equal");
    free(dup);
}
END_TEST

START_TEST(util_exec) {
    char *cmd_false = "false", *cmd_true = "true";
    pid_t pid;
    int status, ec;

    pid = exec(cmd_false);
    ec = waitpid(pid, &status, 0);
    if (ec == -1) fail("waitpid() returned -1");
    if (WEXITSTATUS(status) != 1) {
        fail("expected 'false' to exit with 1");
    }

    pid = exec(cmd_true);
    ec = waitpid(pid, &status, 0);
    if (ec == -1) fail("waitpid() returned -1");
    if (WEXITSTATUS(status) != 0) {
        fail("expected 'true' to exit with 0");
    }
}
END_TEST

START_TEST(util_set_process_name) {
    char *name = "my process name";
    set_process_name(name);
    char buffer[MAX_PROC_NAME_LEN];
    prctl(PR_GET_NAME, buffer);
    if (!streq(name, buffer)) fail("process names are not the same");
}
END_TEST

static Suite * test_suite() {
    Suite *ret = suite_create("util_suite");
    TCase *tc_util = tcase_create("util_testcases");
    tcase_add_test(tc_util, util_trim);
    tcase_add_test(tc_util, util_streq);
    tcase_add_test(tc_util, util_parse_command);
    tcase_add_test(tc_util, util_exec);
    tcase_add_test(tc_util, util_set_process_name);
    suite_add_tcase(ret, tc_util);
    return ret;
}

int main(void) {
    int failures = 0;
    Suite *s = test_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    failures = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

