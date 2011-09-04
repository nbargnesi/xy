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

START_TEST(core_foreach_in_list) {
    int i = 0;
}
END_TEST

static Suite * test_suite() {
    Suite *ret = suite_create("core_suite");
    TCase *tc_util = tcase_create("core_testcases");
    tcase_add_test(tc_util, core_foreach_in_list);
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

