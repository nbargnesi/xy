/*
 * Copyright 2011-2012 Nick Bargnesi <nick@den-4.com>. All rights reserved.
 *
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

#include "broadcast.h"
#include "util.h"
#include "logging.h"
#include "xy.h"

#include <pthread.h>
#include <sys/prctl.h>

struct BroadcastEndpoint {
    int sd;
    struct sockaddr *dest;
    struct sockaddr_in group_sckt;
    uint size;
};
typedef struct BroadcastEndpoint BCAST_ENDPT;

struct message_list_element {
    char *msg;
    struct message_list_element *next;
};
typedef struct message_list_element MESSAGE;

struct message_list {
    MESSAGE *head;
    MESSAGE *tail;
};
typedef struct message_list MESSAGES;

static BCAST_ENDPT *endpt;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static MESSAGES *messages;

static void * pthread_broadcast_send() {
    set_process_name("xy: broadcast");
    int len;
    char *buffer, *padded_buffer;
    ssize_t rc;
    for (;;) {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);

        if (messages == NULL) {
            pthread_mutex_unlock(&mutex);
            continue;
        }

        MESSAGE *current, *message = messages->head;
        while (message) {
            buffer = message->msg;
            len = strlen(buffer);
            padded_buffer = malloc(BROADCAST_LENGTH);
            sprintf(padded_buffer, "%s%*s", buffer, (BROADCAST_LENGTH - (len + 1)), "");
            rc = sendto(endpt->sd, padded_buffer, len, 0, endpt->dest, endpt->size); 
            if (rc == -1) {
                const char *msg = "failed to broadcast message, shutting down?";
                log_warn(globals->log, msg);
            }
            current = message;
            message = message->next;
            free(padded_buffer);
            free(current->msg);
            free(current);
        }
        free(messages);
        messages = NULL;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

bool broadcast_init(const char *address, const uint port) {
    endpt = malloc(sizeof(BCAST_ENDPT));
    memset(endpt, 0, sizeof(BCAST_ENDPT));
    struct in_addr local_iface;

    endpt->sd = socket(AF_INET, SOCK_DGRAM, 0);
    if (endpt->sd < 0) {
        perror("Error opening datagram socket");
        return false;
    }

    memset((char*) &endpt->group_sckt, 0, sizeof(endpt->group_sckt));
    endpt->group_sckt.sin_family = AF_INET;
    endpt->group_sckt.sin_addr.s_addr = inet_addr(address);
    endpt->group_sckt.sin_port = htons(port);
    local_iface.s_addr = inet_addr("127.0.0.1");

    int len = sizeof(local_iface);
    const void *optval = (char *) &local_iface;
    int rc = setsockopt(endpt->sd, IPPROTO_IP, IP_MULTICAST_IF, optval, len);
    if (rc < 0) {
        perror("Error setting local interface");
        return false;
    }

    endpt->size = sizeof(endpt->group_sckt);
    endpt->dest = (struct sockaddr *) &endpt->group_sckt;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_t thread;
    pthread_create(&thread, NULL, &pthread_broadcast_send, NULL);

    register_shutdown_hook("broadcast", broadcast_terminate);
    return true;
}

void broadcast_send(const char *msg) {
    if (!msg) return;
    pthread_mutex_lock(&mutex);

    MESSAGE *message;
    if (messages == NULL) {
        messages = malloc(sizeof(MESSAGES));
        message = malloc(sizeof(MESSAGE));
        message->msg = strdup(msg);
        message->next = NULL;
        messages->head = messages->tail = message;
    } else {
        message = malloc(sizeof(MESSAGE));
        message->msg = strdup(msg);
        message->next = NULL;
        messages->tail->next = message;
        messages->tail = message;
    }

    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond);
}

void broadcast_terminate() {
    if (endpt) {
        close(endpt->sd);
        free(endpt);
    }
}

