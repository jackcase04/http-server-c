#include "processor.h"

const char *html =
    "<!DOCTYPE html>"
    "<html>"
    "<head><title>C Server</title></head>"
    "<body><h1>Hello from C</h1></body>"
    "</html>"
;

void process_request(server_connection *server, const char request[]) {
    // For now, we'll just have this hardcoded here.
    // We can move this later
    // Assemble a basic hardcoded HTML file

    char message[1024];
    int message_len = sizeof(message);

    printf("Client socket: %d\n", server->client_socket);
    printf("Request:\n%s\n", request);

    // What were gonna need to do here is parse:
    // 1. The method
    // 2. The path/resource
    // and decide what to do.
    char tokens[64][256];
    split(request, 4096, tokens, ' ');

    decide_response(tokens, message, &message_len);

    send(server->client_socket, message, message_len, 0);

    printf("HTML message sent\n");
}

void decide_response(char tokens[][256], char message[], int *message_len) {
    // First, parse method
    // For now we only implement GET
    printf("Token 0: %s\n", tokens[0]);

    if (strcmp(tokens[0], "GET") == 0) {
        printf("Received a GET request.");

        // Use snprintf() function to construct our HTTP response with our "HTML file"
        // (%zu is replaced with strlen(html), and %s is replaced with html)
        *message_len = snprintf(message, *message_len,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %zu\r\n"
            "\r\n"
            "%s",
            strlen(html), html
        );
    }
}

void split(const char input[], int len, char output[][256], char delimeter) {
    int token_num = 0;
    int indiv_index = 0;

    for (int i = 0; i < len; ++i) {
        if (input[i] == delimeter) {
            token_num++;
            indiv_index = 0;
        }

        output[token_num][indiv_index] = input[i];

        indiv_index++;
    }
}