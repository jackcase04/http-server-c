#include "data.h"

File_instance *get_resource(char string[]) {
    transform_path(string);

    printf("Trying to open %s\n", string);
    FILE *ptr = fopen(string, "rb");

    // If the file cannot be opened, NULL is returned and no memory is allocated
    if (ptr != NULL) {
        File_instance *file = malloc(sizeof(File_instance));

        // Get the length of the file using fseek()
        fseek(ptr, 0, SEEK_END);
        file->file_buff_len = ftell(ptr);
        // Seek back to beginning
        fseek(ptr, 0L, SEEK_SET);

        file->data = malloc(file->file_buff_len);

        fread(file->data, 1, file->file_buff_len, ptr);
        fclose(ptr);

        return file;
    } else {
        return NULL;
    }
}

File_instance *get_file_size(char string[]) {
    transform_path(string);

    FILE *ptr = fopen(string, "rb");
    
    if (ptr != NULL) {
        File_instance *file = malloc(sizeof(File_instance));
        file->data = NULL;

        // Get the length of the file using fseek()
        fseek(ptr, 0, SEEK_END);
        file->file_buff_len = ftell(ptr);

        fclose(ptr);

        return file;
    } else {
        return NULL;
    }
}

int transform_path(char string[]) {
    if (strcmp(string, "/") == EXIT_SUCCESS) {
        strcpy(string, "public/index.html");
        return EXIT_SUCCESS;
    } else if (strcmp(string, "/login") == EXIT_SUCCESS) {
        strcpy(string, "public/login.html");
        return EXIT_SUCCESS;
    } else {
        // printf("Did not transform.\n");
    }

    return EXIT_SUCCESS;
}