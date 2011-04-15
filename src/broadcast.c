#include "broadcast.h"

#include <pthread.h>

struct BroadcastEndpoint {
    int sd;
    struct sockaddr *dest;
    struct sockaddr_in group_sckt;
    uint size;
};
typedef struct BroadcastEndpoint BCAST_ENDPT;

static BCAST_ENDPT *endpt;

static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static char *send_buffer;

static void * pthread_broadcast_send() {
    int len;
    char *buffer;
    ssize_t rc;
    for (;;) {
        pthread_cond_wait(&cond, &cond_mutex);
        pthread_mutex_lock(&mutex);
        buffer = strdup(send_buffer);
        free(send_buffer);
        len = strlen(buffer);
        rc = sendto(endpt->sd, buffer, len, 0, endpt->dest, endpt->size); 
        pthread_mutex_unlock(&mutex);
        free(buffer);
        if (rc == -1) DIE;
    }
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

    if (setsockopt(endpt->sd, IPPROTO_IP, IP_MULTICAST_IF, (char *) &local_iface, sizeof(local_iface)) < 0) {
        perror("Error setting local interface");
        return false;
    }

    endpt->size = sizeof(endpt->group_sckt);
    endpt->dest = (struct sockaddr *) &endpt->group_sckt;

    pthread_mutex_init(&cond_mutex, NULL);
    pthread_mutex_init(&mutex, NULL);
    pthread_t thread;
    pthread_create(&thread, NULL, &pthread_broadcast_send, NULL);

    return true;
}

void broadcast_send(const char *msg) {
    if (!msg) return;
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    send_buffer = strdup(msg);    
    pthread_mutex_unlock(&mutex);
}

void broadcast_terminate() {
    if (endpt) free(endpt);
}

