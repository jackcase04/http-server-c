#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

#define PORT 8080

int main(void) {

    // First step is to create the socket and get a descriptor using the socket function.
    // use AF_INET
    // SOCK_STREAM = TCP, SOCK_DGRAM = UDP
    // Protocol for IP = 0
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    if (server_socket < 0) {
        perror("Creating socket failed");
        exit(0);
    }

    // optional step to prevent address already in use error
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(0);
    }

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Next bind the socket to the address and port we have defined in address.
    // This is INADDR_ANY for IP, and PORT (8080) for port
    // (Cast server address to generic sockaddr)
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        exit(0);
    }

    // Listen is neccessary to set the server socket in passive mode
    if (listen(server_socket, 3) < 0) {
        perror("Listen failed");
        exit(0);
    }
    
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

    int client_socket;

    while (1) {
        // Accept function blocks until a connection has arrived
        // Creates a new connected socket and a file descriptor to it
        printf("Waiting for a new connection...\n");

        socklen_t client_addrlen = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_addrlen);

        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Client socket: %d\n", client_socket);

        send(client_socket, message, len, 0);
        printf("HTML message sent\n");

        close(client_socket);
    }

    close(server_socket);

    return 0;
}