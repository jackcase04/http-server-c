#include "data.h"

#include <stdlib.h>

File_instance *get_resource(char string[]) {
    
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

HTTP_Status resolve_path(char string[]) {

    char full_path[PATH_MAX];
    snprintf(full_path, sizeof(full_path), "%s%s", WEB_ROOT, string);

    char resolved[PATH_MAX];
    // If a real absolute path cant be found, it will return NULL
    if (realpath(full_path, resolved) == NULL) {
        return NOT_FOUND;
    }

    strcpy(string, resolved);

    // Check it's within web root
    if (strncmp(resolved, WEB_ROOT, strlen(WEB_ROOT)) != EXIT_SUCCESS) {
        printf("Path traversal attempt!\n\n");
        return FORBIDDEN;
    }

    // If we made it to here, resolved is our true path we are going to read from.
    // It should prevent any attempt to escape the public folder.
    // Transform / to /index.html
    if (strcmp(resolved, WEB_ROOT) == 0) {
        snprintf(string, PATH_MAX+11, "%s/index.html", WEB_ROOT);
    } else {
        strcpy(string, resolved);
    }

    return OK;
}