#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <poll.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "processor.h"
#include "types.h"

Server setup_server();

void start_server(Server *server);

#endif