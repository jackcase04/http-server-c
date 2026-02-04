#include "processor.h"

void process_request(server_connection *server, const char request[]) {
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

    printf("Client socket: %d\n", server->client_socket);
    printf("Request:\n%s\n", request);

    // What were gonna need to do here is parse:
    // 1. The method
    // 2. The path/resource
    // and decide what to do.

    send(server->client_socket, message, len, 0);
    printf("HTML message sent\n");
}