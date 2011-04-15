#ifndef BROADCAST_H_
#define BROADCAST_H_

#include "core.h"

/**
 * Define: BROADCAST_LENGTH
 *
 * Broadcast length.
 */
#define BROADCAST_LENGTH 256

/*
 * Function: broadcast_init
 *
 * Initializes the broadcast module.
 *
 * The multicast group specified by <CFG_BROADCAST_GROUP> and
 * <CFG_BROADCAST_PORT> is joined and a thread is created to process broadcast
 * messages.
 */
bool broadcast_init(const char *, const uint);

/*
 * Function: broadcast_send
 *
 * Broadcasts a message to the multicast group.
 *
 * See <broadcast_init>.
 */
void broadcast_send(const char *);

/*
 * Function: broadcast_terminate
 *
 * Terminates the broadcast module and closes the multicast connection.
 */
void broadcast_terminate();

#endif

