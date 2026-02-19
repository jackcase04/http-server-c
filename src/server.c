#include "server.h"

const int TIMEOUT_TIME = 100;

server_connection setup_server() {
    server_connection server;

    // First step is to create the socket and get a descriptor using the socket function.
    // use AF_INET
    // SOCK_STREAM = TCP, SOCK_DGRAM = UDP
    // Protocol for IP = 0
    server.server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if (server.server_socket < 0) {
        perror("Creating socket failed");
        exit(0);
    }

    // optional step to prevent address already in use error
    int opt = 1;
    if (setsockopt(server.server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(0);
    }

    // struct sockaddr_in server_address;
    // struct sockaddr_in client_address;

    server.server_address.sin_family = AF_INET;
    server.server_address.sin_addr.s_addr = INADDR_ANY;
    server.server_address.sin_port = htons(PORT);

    // Next bind the socket to the address and port we have defined in address.
    // This is INADDR_ANY for IP, and PORT (8080) for port
    // (Cast server_connection address to generic sockaddr)
    if (bind(server.server_socket, (struct sockaddr*)&server.server_address, sizeof(server.server_address)) < 0) {
        perror("Bind failed");
        exit(0);
    }

    // Listen is neccessary to set the server_connection socket in passive mode
    if (listen(server.server_socket, 3) < 0) {
        perror("Listen failed");
        exit(0);
    }

    return server;
}

void start_server(server_connection *server) {
    ssize_t bytes_read;
    char request[4096];

    printf("Server listening on IP: %s Port: %d\n\n", inet_ntoa(server->server_address.sin_addr), PORT);

    while (1) {
        // Accept function blocks until a connection has arrived
        // Creates a new connected socket and a file descriptor to it
        socklen_t client_addrlen = sizeof(server->client_address);

        server->client_socket = accept(server->server_socket, (struct sockaddr*)&server->client_address, &client_addrlen);

        if (server->client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        struct pollfd pfd = { .fd = server->client_socket, .events = POLLIN };
        int ready = poll(&pfd, 1, TIMEOUT_TIME);

        if (ready != 0 && pfd.revents & POLLIN) {
            bytes_read = recv(server->client_socket, request, sizeof(request) - 1, 0);

            // Null terminate so we can print
            request[bytes_read] = '\0';

            if (bytes_read < 0) {
                perror("Read failed");
                close(server->client_socket);
                continue;
            } else if (bytes_read == 0) {
                close(server->client_socket);
                continue;
            }

            process_request(server, request);

        } else {
            printf("Socket timeout, %d ms with no request\n", TIMEOUT_TIME);
        }

        close(server->client_socket);
    }

    close(server->server_socket);
}