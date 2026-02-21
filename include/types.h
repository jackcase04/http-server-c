#ifndef TYPES_H
#define TYPES_H

#include <netinet/in.h>

#define PORT 8080
#define MESSAGE_SIZE 4096
#define PATH_MAX 4096

extern char WEB_ROOT[PATH_MAX];

typedef struct {
    int client_socket;
    socklen_t client_addrlen;
    char message[4096];
} Request;

typedef struct {
    int server_socket_fd;
    struct sockaddr_in server_address;
    void (*cb_arr[1]) (Request *);
} Server;

typedef enum {
    GET,
    HEAD,
    UNKNOWN
} HTTP_Method;

typedef enum {
    OK,
    NOT_FOUND,
    FORBIDDEN
} HTTP_Status;

typedef struct {
    char *data;
    size_t file_buff_len;
} File_instance;

#endif