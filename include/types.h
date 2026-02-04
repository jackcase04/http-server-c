#ifndef TYPES_H
#define TYPES_H

#include <netinet/in.h>

#define PORT 8080

typedef struct {
    int server_socket;
    struct sockaddr_in server_address;
    int client_socket;
    struct sockaddr_in client_address;
} server_connection;

#endif