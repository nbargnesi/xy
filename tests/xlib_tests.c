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

#include <check.h>
#include <stdio.h>
#include <string.h>
#include "core.h"
#include "monitor.h"
#include "xyxlib.h"
#include "util.h"

#define TEST_DISPLAY Display *d = open_display(); \
                     if (!d) { \
                         fprintf(stderr, \
                                 "no display - this test will not run\n"); \
                         return; \
                     }

START_TEST(display) {
    TEST_DISPLAY;
    close_display(d);
}
END_TEST

START_TEST(list_clients) {
    TEST_DISPLAY;
    Window root = root_window(d);
    CLIENTS_LIST *clients = get_clients(d, root);
    clients_list_free(clients);
    close_display(d);
}
END_TEST

START_TEST(string_to_keysym) {
    KeySym sym = convert_to_keysym("p");
    if (!sym) {
        fail("couldn't get sym");
    }
}
END_TEST

START_TEST(monitor) {
    TEST_DISPLAY;
    if (is_xinerama_active(d)) {
        int numscrn;
        XineramaScreenInfo *screens = get_xinerama_screens(d, &numscrn);
        if (!screens) fail("no Xinerama screens");

        MONITOR *monitors = calloc(numscrn, sizeof(MONITOR));
        for (int i = 0; i < numscrn; i++) {
            monitors[i].ordinal = screens[i].screen_number;
            monitors[i].xorigin = screens[i].x_org;
            monitors[i].yorigin = screens[i].y_org;
            monitors[i].width = screens[i].width;
            monitors[i].height = screens[i].height;
        }
        XFree(screens);
        free(monitors);
    }
    Screen *screen = DefaultScreenOfDisplay(d);
    if (!screen) fail("no default screen");
    MONITOR *monitors = calloc(1, sizeof(MONITOR));
    monitors[0].ordinal = 0;
    monitors[0].xorigin = 0;
    monitors[0].yorigin = 0;
    monitors[0].width = screen->width;
    monitors[0].height = screen->height;
    free(monitors);
    close_display(d);
}
END_TEST

static Suite * test_suite() {
    Suite *ret = suite_create("xlib_suite");
    TCase *tc_xlib = tcase_create("xlib_testcases");
    tcase_add_test(tc_xlib, display);
    tcase_add_test(tc_xlib, list_clients);
    tcase_add_test(tc_xlib, string_to_keysym);
    tcase_add_test(tc_xlib, monitor);
    suite_add_tcase(ret, tc_xlib);
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

