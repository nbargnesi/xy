#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include "types.h"

struct ConfigEntry {
    char *name;
    char *value;
    struct ConfigEntry *next;
};

typedef struct ConfigEntry CONFIG_ENTRY;

struct Config {
    struct ConfigEntry *head;
    struct ConfigEntry *tail;
    uint num_entries;
};

typedef struct Config CONFIG;

CONFIG * get_config(const char *);

CONFIG * empty_config();

void free_config(CONFIG *);

char * get_config_value(CONFIG *, const char *);

#endif

