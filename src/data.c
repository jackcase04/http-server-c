#include "data.h"

int check_resource_exists(const char string[]) {
    if (strcmp(string, "/") == 0) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}

int get_resource(char string[], char file_buff[], int len) {
    FILE *ptr = fopen(string, "rb");

    fread(file_buff, 1, len, ptr);
    fclose(ptr);

    return EXIT_SUCCESS;
}

size_t get_file_size(char string[]) {
    transform_path(string);

    printf("File name: %s\n", string);

    FILE *ptr = fopen(string, "rb");

    // Get the length of the file using fseek()
    fseek(ptr, 0, SEEK_END);
    size_t len = ftell(ptr);

    return len;
}

int transform_path(char string[]) {
    if (strcmp(string, "/") == 0) {
        strcpy(string, "public/index.html");
        return EXIT_SUCCESS;
    } else {
        printf("Did not transform.\n");
    }

    return EXIT_SUCCESS;
}