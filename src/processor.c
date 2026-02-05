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
    split(request, tokens, ' ');

    decide_response(tokens, message, &message_len);

    send(server->client_socket, message, message_len, 0);
}

void decide_response(char tokens[][256], char message[], int *message_len) {
    // First, parse method
    // For now we only implement GET
    if (strcmp(tokens[0], "GET") == 0) {
        // Use snprintf() function to construct our HTTP response with our "HTML file"
        // (%zu is replaced with strlen(html), and %s is replaced with html)
        
        printf("Token 1: <%s>\n", tokens[1]);

        if (strcmp(tokens[1], "/") == 0) {
            *message_len = snprintf(message, *message_len,
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: %zu\r\n"
                "\r\n"
                "%s",
                strlen(html), html
            );
        } else {
            printf("\nNot Found\n");
            const char *body = "Not Found";
            *message_len = snprintf(message, *message_len,
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: %zu\r\n"
                "\r\n"
                "%s",
                strlen(body), body
            );
        }
        
    } else {
        printf("\nNot Implemented\n");
        const char *body = "Not Implemented";
        *message_len = snprintf(message, *message_len,
            "HTTP/1.1 501 Not Implemented\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: %zu\r\n"
            "\r\n"
            "%s",
            strlen(body), body
        );
    }
}

void split(const char input[], char output[][256], char delimeter) {
    int token_num = 0;
    int indiv_index = 0;

    for (int i = 0; input[i] != '\0'; ++i) {
        // Check to ensure token number never exceeds 64
        if (token_num >= 64) {
            break;
        }

        if (input[i] == delimeter) {
            // We have encountered the delimeter.
            // Null terminate the current token, then move to filling the next token
            output[token_num][indiv_index] = '\0';
            token_num++;
            indiv_index = 0;
        } else {
            // Not a delimeter char.
            // Fill the char in the current token string
            if (indiv_index < 255) {
                output[token_num][indiv_index] = input[i];
            }

            indiv_index++;
        }
    }

    if (token_num < 64) {
        output[token_num][indiv_index] = '\0';
    }
}