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
#include "core.h"
#include "monitor.h"
#include "xyxlib.h"

START_TEST(display) {
    Display *d = open_display();
    if (!d) fail("no display");
    close_display(d);
}
END_TEST

START_TEST(monitor) {
    Display *d = open_display();
    if (!d) fail("no display");
    if (is_xinerama_active(d)) {
        int numscrn;
        XineramaScreenInfo *screens = get_xinerama_screens(d, &numscrn);
        if (!screens) fail("no Xinerama screens");
        fprintf(stderr, "Xinerama says %d screens are here\n", numscrn);

        MONITOR *monitors = calloc(numscrn, sizeof(MONITOR));
        for (int i = 0; i < numscrn; i++) {
            monitors[i].ordinal = screens[i].screen_number;
            monitors[i].xorigin = screens[i].x_org;
            monitors[i].yorigin = screens[i].y_org;
            monitors[i].width = screens[i].width;
            monitors[i].height = screens[i].height;

            fprintf(stderr, "Monitor #%d\n", monitors[i].ordinal);
            fprintf(stderr, "\tx-origin: %d\n", monitors[i].xorigin);
            fprintf(stderr, "\ty-origin: %d\n", monitors[i].yorigin);
            fprintf(stderr, "\twidth: %d\n", monitors[i].width);
            fprintf(stderr, "\theight: %d\n", monitors[i].height);
        }
        XFree(screens);
        free(monitors);
    }
    Screen *screen = DefaultScreenOfDisplay(d);
    if (!screen) fail("no default screen");
    fprintf(stderr, "default screen found\n");
    MONITOR *monitors = calloc(1, sizeof(MONITOR));
    monitors[0].ordinal = 0;
    monitors[0].xorigin = 0;
    monitors[0].yorigin = 0;
    monitors[0].width = screen->width;
    monitors[0].height = screen->height;
    fprintf(stderr, "Monitor #%d\n", monitors[0].ordinal);
    fprintf(stderr, "\tx-origin: %d\n", monitors[0].xorigin);
    fprintf(stderr, "\ty-origin: %d\n", monitors[0].yorigin);
    fprintf(stderr, "\twidth: %d\n", monitors[0].width);
    fprintf(stderr, "\theight: %d\n", monitors[0].height);
    free(monitors);
    close_display(d);
}
END_TEST

static Suite * test_suite() {
    Suite *ret = suite_create("xy");
    TCase *tc_xlib = tcase_create("xlib");
    tcase_add_test(tc_xlib, display);
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

