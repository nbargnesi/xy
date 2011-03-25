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
