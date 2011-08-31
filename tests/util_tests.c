/*
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

#include <check.h>
#include <stdio.h>
#include <string.h>
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

static Suite * test_suite() {
    Suite *ret = suite_create("util_suite");
    TCase *tc_util = tcase_create("util_testcases");
    tcase_add_test(tc_util, util_trim);
    tcase_add_test(tc_util, util_streq);
    tcase_add_test(tc_util, util_parse_command);
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

