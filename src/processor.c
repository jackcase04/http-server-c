#include <stdlib.h>

#include "processor.h"

void process_request(Request *request) {
    char tokens[64][256];
    split(request->message, tokens, ' ');

    char headers[1024];
    int headers_len = sizeof(headers);
    
    int HTTP_response_code = 0;
    char HTTP_message[64];
    File_instance *file = NULL;

    HTTP_Method method = get_http_method(tokens[0]);
    char *path = tokens[1];

    switch (method) {
        case GET:
            switch (resolve_path(path))
            {
            case OK:
                // Get the resource and read it into the file buffer
                // get_resource allocates the required memory for the file
                file = get_resource(path);

                if (file != NULL) {
                    HTTP_response_code = 200;
                    strcpy(HTTP_message, "OK");
                } else {
                    HTTP_response_code = 404;
                    strcpy(HTTP_message, "Not Found");
                }

                break;
            
            case NOT_FOUND:
                HTTP_response_code = 404;
                strcpy(HTTP_message, "Not Found");

                break;

            case FORBIDDEN:
                HTTP_response_code = 404;
                strcpy(HTTP_message, "Forbidden");

                break;

            default:
                break;
            }

            break;
        case HEAD:
            switch (resolve_path(path))
            {
            case OK:
                file = get_file_size(path);

                if (file != NULL) {
                    HTTP_response_code = 200;
                    strcpy(HTTP_message, "OK");
                } else {
                    HTTP_response_code = 404;
                    strcpy(HTTP_message, "Not Found");
                }

                break;
            
            case NOT_FOUND:
                HTTP_response_code = 404;
                strcpy(HTTP_message, "Not Found");

                break;

            case FORBIDDEN:
                HTTP_response_code = 404;
                strcpy(HTTP_message, "Forbidden");

                break;

            default:
                break;
            }

            break;
        default:
            HTTP_response_code = 501;
            strcpy(HTTP_message, "Not Implemented");
    }

    printf("Method Received: %s\n", tokens[0]);
    printf("Request Path: %s\n\n", path);

    printf("Returning: %d %s\n\n", HTTP_response_code, HTTP_message);

    headers_len = snprintf(headers, headers_len,
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n",
        HTTP_response_code, HTTP_message, file != NULL? file->file_buff_len : 0
    );

    send(request->client_socket, headers, headers_len, 0);

    if (file != NULL) {
        // If there is a body to send, send it too
        if (file->data != NULL) {
            send(request->client_socket, file->data, file->file_buff_len, 0);
            free(file->data);
        }

        free(file);
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

HTTP_Method get_http_method(char string[]) {
    if (strcmp(string, "GET") == EXIT_SUCCESS) return GET;
    if (strcmp(string, "HEAD") == EXIT_SUCCESS) return HEAD;
    return UNKNOWN;
}