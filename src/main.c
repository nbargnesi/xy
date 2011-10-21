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

#include "state.h"
#include "lifecycle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    size_t length = 0; 
    for (int i = 0; i < argc; i++) {
        length += strlen(argv[i]);
    }

    uint bytes = length + (argc - 1) + 1;
    char *cmd = malloc(bytes);
    memset(cmd, 0, bytes);

    for (int i = 0; i < argc; i++) {
        if (i != 0) {
            strcat(cmd, " ");
        }
        strcat(cmd, argv[i]);
    }

    xy_init(cmd);
}

