#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include "processor.h"

#define PORT 8080

struct server_connection {
    int server_socket;
    struct sockaddr_in server_address;
    int client_socket;
    struct sockaddr_in client_address;
};

struct server_connection setup_server();

void start_server(struct server_connection server);

#endif