#ifndef TYPES_H
#define TYPES_H

#include <netinet/in.h>

#define PORT 8080

// For now only one event type
enum EventType { EventTypeRequest };

typedef struct {
    int client_socket;
    struct sockaddr_in client_address;
    char message[4096];
} Request;

typedef struct {
    int server_socket;
    struct sockaddr_in server_address;
    void (*cb_arr[1]) (Request *);
} Server;

#endif