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

#ifndef SYSINFO_H_
#define SYSINFO_H_

#include "core.h"

typedef struct {
    uint total;
    uint actual;
} CPUINFO;

typedef struct {
    uint total_kb;
    uint free_kb;
} MEMINFO;

CPUINFO * get_cpu_info();
float get_cpu_usage(const CPUINFO *, const CPUINFO *);
MEMINFO * get_mem_info();
void sysinfo_cpu_free(CPUINFO *);
void sysinfo_mem_free(MEMINFO *);

#endif
