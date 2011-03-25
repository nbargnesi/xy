#include "configuration.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static char * left_trim(char *str) {
    while (isspace(*str)) str++;
    return str;
}

static char * right_trim(char *str) {
    char *c = str + strlen(str);
    while (isspace(*--c));
    *(c + 1) = '\0';
    return str;
}

static char * trim(char *str) {
    return right_trim(left_trim(str));
}

CONFIG * get_config(const char *pathname) {

    FILE *fp = fopen(pathname, "r");
    if (!fp) return NULL;

    char *line = NULL;
    char *name = NULL;
    char *value = NULL;
    size_t len = 0;

    CONFIG *cfg = empty_config();
    while (getline(&line, &len, fp) != -1) {

        // Tokenize the line using '=' as the delimiter.
        char *token = strtok(line, "=");
        if (!token) continue;

        // Trim leading/trailing whitespace.
        name = trim(token);

        // Skip comments
        if (*name == '#') continue;
        token = strtok(NULL, "=");
        if (!token) continue;
        value = trim(token);

        // Create a CONFIG_ENTRY for the name/value tokens.
        CONFIG_ENTRY *ce = malloc(sizeof(CONFIG_ENTRY));
        memset(ce, 0, sizeof(CONFIG_ENTRY));
        ce->name = strdup(name);
        ce->value = strdup(value);
        ce->next = NULL;

        // If no entries exist...
        if (cfg->num_entries == 0) {
            // ce becomes head and tail
            cfg->head = ce;
            cfg->tail = ce;
        } else {
            // ce becomes tail
            cfg->tail->next = ce;
            cfg->tail = ce;
        }
        cfg->num_entries++;
    }

    free(line);
    fclose(fp);
    return cfg;
}

CONFIG * empty_config() {
    CONFIG *cfg = malloc(sizeof(CONFIG));
    memset(cfg, 0, sizeof(CONFIG));
    cfg->head = NULL;
    cfg->tail = NULL;
    cfg->num_entries = 0;
    return cfg;
}

void free_config(CONFIG *cfg) {
    if (cfg) {
        for (CONFIG_ENTRY *c = cfg->head; c; ) {
            free(c->name);
            free(c->value);
            CONFIG_ENTRY *next = c->next;
            free(c);
            c = next;
        }
        free(cfg);
    }
}

char *get_config_value(CONFIG *cfg, const char *name) {
    for (CONFIG_ENTRY *c = cfg->head; c; c = c->next) {
        if (strcmp(c->name, name) == 0) return c->value;
    }
    return NULL;
}

