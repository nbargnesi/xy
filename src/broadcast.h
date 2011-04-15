#ifndef BROADCAST_H_
#define BROADCAST_H_

#include "core.h"

bool broadcast_init(const char *, const uint);

void broadcast_send(const char *);

void broadcast_terminate();

#endif

