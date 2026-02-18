#include "processor.h"

void process_request(server_connection *server, const char request[]) {

    printf("Client socket: %d\n", server->client_socket);
    printf("Request:\n%s\n", request);

    char tokens[64][256];
    split(request, tokens, ' ');

    decide_response(server, tokens);

    // send(server->client_socket, message, message_len, 0);
}

void decide_response(server_connection *server, char tokens[][256]) {
    char headers[1024];
    int headers_len = sizeof(headers);
    // First, parse method
    // For now we only implement GET and HEAD
    char *path = tokens[1];

    if (strcmp(tokens[0], "GET") == 0) {

        if (check_resource_exists(path) == EXIT_SUCCESS) {

            size_t file_buff_len = get_file_size(path);
            printf("File size: %zu\n", file_buff_len);

            // Allocate enough memory for the file
            char *file_buf = malloc(file_buff_len);
            get_resource(path, file_buf, file_buff_len);

            headers_len = snprintf(headers, headers_len,
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: %zu\r\n"
                "\r\n",
                strlen(file_buf)
            );

            send(server->client_socket, headers, headers_len, 0);
            send(server->client_socket, file_buf, file_buff_len, 0);

            free(file_buf);

        } else {
            const char body[] = "Not Found";
            headers_len = snprintf(headers, headers_len,
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: %zu\r\n"
                "\r\n"
                "%s",
                strlen(body), body
            );

            send(server->client_socket, headers, headers_len, 0);
        }
        
    } else if (strcmp(tokens[0], "HEAD") == 0) {
        printf("Token 1: <%s>\n", tokens[1]);

        if (check_resource_exists(path) == EXIT_SUCCESS) {
            size_t file_buff_len = get_file_size(path);

            headers_len = snprintf(headers, headers_len,
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: %zu\r\n"
                "\r\n",
                file_buff_len
            );

            send(server->client_socket, headers, headers_len, 0);
        } else {
            printf("\nNot Found\n");
            const char *body = "Not Found";
            headers_len = snprintf(headers, headers_len,
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: %zu\r\n"
                "\r\n",
                strlen(body)
            );

            send(server->client_socket, headers, headers_len, 0);
        }
    } else {
        printf("\nNot Implemented\n");
        const char *body = "Not Implemented";
        headers_len = snprintf(headers, headers_len,
            "HTTP/1.1 501 Not Implemented\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: %zu\r\n"
            "\r\n"
            "%s",
            strlen(body), body
        );

        send(server->client_socket, headers, headers_len, 0);
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