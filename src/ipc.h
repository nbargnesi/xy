#ifndef IPC_H_
#define IPC_H_

#include "core.h"

/**
 * Define: IPC_MSG_LENGTH
 *
 * IPC message length.
 */
#define IPC_MSG_LENGTH 256

/*
 * Function: ipc_init
 *
 * Initializes the IPC module.
 */
bool ipc_init();

/*
 * Function: broadcast_terminate
 *
 * Terminates the broadcast module and closes the multicast connection.
 */
void ipc_terminate();

#endif

