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

