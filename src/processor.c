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
    char *path = tokens[1];
    char *file_buf = NULL;
    size_t file_buff_len = 0;
    int HTTP_response_code = 0;
    char HTTP_message[64];

    if (strcmp(tokens[0], "GET") == 0) {

        if (check_resource_exists(path) == EXIT_SUCCESS) {

            file_buff_len = get_file_size(path);

            // Allocate enough memory for the file
            file_buf = malloc(file_buff_len);
            get_resource(path, file_buf, file_buff_len);

            HTTP_response_code = 200;
            strcpy(HTTP_message, "OK");
        } else {
            HTTP_response_code = 404;
            strcpy(HTTP_message, "Not Found");
        }
        
    } else if (strcmp(tokens[0], "HEAD") == 0) {

        if (check_resource_exists(path) == EXIT_SUCCESS) {
            file_buff_len = get_file_size(path);

            HTTP_response_code = 200;
            strcpy(HTTP_message, "OK");
        } else {
            HTTP_response_code = 404;
            strcpy(HTTP_message, "Not Found");
        }
    } else {
        HTTP_response_code = 501;
        strcpy(HTTP_message, "Not Implemented");
    }

    headers_len = snprintf(headers, headers_len,
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n",
        HTTP_response_code, HTTP_message, file_buff_len
    );

    send(server->client_socket, headers, headers_len, 0);

    // If there is a body to send, send it too
    if (file_buf != NULL) {
        send(server->client_socket, file_buf, file_buff_len, 0);
        free(file_buf);
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