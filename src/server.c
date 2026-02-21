#include "server.h"

const int TIMEOUT_TIME = 100;

Server setup_server() {
    Server server;

    // First step is to create the socket and get a descriptor using the socket function.
    // use AF_INET
    // SOCK_STREAM = TCP, SOCK_DGRAM = UDP
    // Protocol for IP = 0
    server.server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (server.server_socket_fd < 0) {
        perror("Creating socket failed");
        exit(0);
    }

    // optional step to prevent address already in use error
    int opt = 1;
    if (setsockopt(server.server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(0);
    }

    server.server_address.sin_family = AF_INET;
    server.server_address.sin_addr.s_addr = INADDR_ANY;
    server.server_address.sin_port = htons(PORT);

    // Next bind the socket to the address and port we have defined in address.
    // This is INADDR_ANY for IP, and PORT (8080) for port
    // (Cast Server address to generic sockaddr)
    if (bind(server.server_socket_fd, (struct sockaddr*)&server.server_address, sizeof(server.server_address)) < 0) {
        perror("Bind failed");
        exit(0);
    }

    // Listen is neccessary to set the Server socket in passive mode
    if (listen(server.server_socket_fd, 3) < 0) {
        perror("Listen failed");
        exit(0);
    }

    return server;
}

void start_server(Server *server) {
    ssize_t bytes_read;

    int epoll_fd = epoll_create1(0);

    if (epoll_fd == -1) {
        perror("epoll_create1 failed");
        exit(EXIT_FAILURE);
    }

    struct epoll_event ev;
    struct epoll_event events[10];

    // Start with only the server fd on the epoll watch list
    ev.events = EPOLLIN;
    ev.data.fd = server->server_socket_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server->server_socket_fd, &ev) < 0) {
        perror("Adding server to watch list failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on IP: %s Port: %d\n\n", inet_ntoa(server->server_address.sin_addr), PORT);

    while (1) {
        // Wait here for 1 or more events to be raised
        int nfds = epoll_wait(epoll_fd, events, 10, -1);

        for (int i = 0; i < nfds; ++i) {

            if (events[i].events & EPOLLIN) {

                if (events[i].data.fd == server->server_socket_fd) {
                    // If the server triggered an event, then a new client has came in
                    // We must add the new client to the watch list

                    struct sockaddr client_address;
                    socklen_t client_addrlen = sizeof(struct sockaddr);
                    // Accept function blocks until a connection has arrived
                    // Creates a new connected socket and a file descriptor to it
                    int client_socket = accept(server->server_socket_fd, (struct sockaddr*)&client_address, &client_addrlen);

                    if (client_socket < 0) {
                        perror("Accept failed");
                        continue;
                    }

                    struct epoll_event ev;
                    ev.events = EPOLLIN;
                    ev.data.fd = client_socket;

                    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &ev);

                } else {
                    // The event raised is a client.
                    // This means that a request is incoming, and ready to be processed
                    Request request;
                    request.client_socket = events[i].data.fd;
                    request.client_addrlen = sizeof(struct sockaddr_in);
                    memset(request.message, 0, MESSAGE_SIZE);

                    // Read the data into the requests message buffer
                    bytes_read = recv(request.client_socket, request.message, sizeof(request.message) - 1, 0);

                    if (bytes_read < 0) {
                        perror("Read failed");
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, request.client_socket, NULL);
                        close(request.client_socket);
                        continue;
                    } else if (bytes_read == 0) {
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, request.client_socket, NULL);
                        close(request.client_socket);
                        continue;
                    }
                    
                    // Null terminate so we can print
                    request.message[bytes_read] = '\0';

                    process_request(&request);
                    // Remove the specific client from the watch list (close behavior)
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, request.client_socket, NULL);
                    close(request.client_socket);
                }
            }
        }
    }
}