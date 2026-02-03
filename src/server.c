#include "server.h"

struct server_connection setup_server() {
    struct server_connection server;

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

void start_server(struct server_connection server) {

    // For now, we'll just have this hardcoded here.
    // We can move this later
    // Assemble a basic hardcoded HTML file
    const char *html =
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>C Server</title></head>"
        "<body><h1>Hello from C</h1></body>"
        "</html>"
    ;

    char message[1024];

    // Use snprintf() function to construct our HTTP response with our "HTML file"
    // (%zu is replaced with strlen(html), and %s is replaced with html)
    int len = snprintf(message, sizeof(message),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %zu\r\n"
        "\r\n"
        "%s",
        strlen(html), html
    );

    char request[4096];
    ssize_t bytes_read;

    while (1) {
        // Accept function blocks until a connection has arrived
        // Creates a new connected socket and a file descriptor to it
        printf("Waiting for a new connection...\n");

        socklen_t client_addrlen = sizeof(server.client_address);
        server.client_socket = accept(server.server_socket, (struct sockaddr*)&server.client_address, &client_addrlen);

        if (server.client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        bytes_read = recv(server.client_socket, request, sizeof(request) - 1, 0);

        if (bytes_read <= 0) {
            perror("Read failed");
            close(server.client_socket);
            continue;
        }

        // Null terminate so we can print
        request[bytes_read] = '\0';

        printf("Client socket: %d\n", server.client_socket);
        printf("Request:\n%s\n", request);

        process_request(request);

        send(server.client_socket, message, len, 0);
        printf("HTML message sent\n");

        close(server.client_socket);
    }

    close(server.server_socket);
}