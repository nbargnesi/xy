#include "sysinfo.h"

#define PROC_STAT "/proc/stat"
#define PROC_STAT_FORMAT "%s\t%d %d %d %d %d %d %d %d %d %d\n"
#define PROC_MEMINFO "/proc/meminfo"
#define PROC_MEMINFO_MEMTOTAL "%s\t%d %s\n"
#define PROC_MEMINFO_MEMFREE "%s\t%d %s\n"

static CPUINFO * ci_init() {
    CPUINFO *ret = malloc(sizeof(CPUINFO));
    return ret;
}

static MEMINFO * mi_init() {
    MEMINFO *ret = malloc(sizeof(MEMINFO));
    return ret;
}

void cpuinfo_free(CPUINFO *ci) {
    free(ci);
}

void meminfo_free(MEMINFO *mi) {
    free(mi);
}

CPUINFO * get_cpu_info() {
    CPUINFO *ret = ci_init();
    char cpu[3];
    uint buf[10];

    FILE *f = fopen(PROC_STAT, "r");
    fscanf(f, PROC_STAT_FORMAT, cpu, buf, buf + 1, buf + 2, buf + 3, buf + 4,
                                buf + 5, buf + 6, buf + 7, buf + 8, buf + 9);
    fclose(f);

    ret->actual = buf[0] + buf[1] + buf[2];
    ret->total = ret->actual + buf[3] + buf[4] + buf[5] +
                 buf[6] + buf[7] + buf[8] + buf[9];
    return ret;
}

float get_cpu_usage(const CPUINFO *ci1, const CPUINFO *ci2) {
    uint actual = ci2->actual - ci1->actual;
    uint total = ci2->total - ci1->total;
    return ((float) actual / (float) total) * 100;
}

MEMINFO * get_mem_info() {
    MEMINFO *ret = mi_init();
    char desc[9], kb[2];
    uint buf[2];

    FILE *f = fopen(PROC_MEMINFO, "r");
    fscanf(f, PROC_MEMINFO_MEMTOTAL, desc, buf, kb);
    fscanf(f, PROC_MEMINFO_MEMFREE, desc, buf + 1, kb);
    fclose(f);
    ret->total_kb = buf[0];
    ret->free_kb = buf[1];
    return ret;
}
