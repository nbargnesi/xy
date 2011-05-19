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

/*
 * TEST HARNESS MAIN FUNCTION
 */
int main(void) {
    fprintf(stderr, "%s\n", __FILE__);
    Display *d = open_display();
    if (!d) fail("no display");
    fail("display");
    return 0;
}

