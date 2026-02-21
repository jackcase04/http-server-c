#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <string.h>
#include "types.h"

// IMPORTANT: This function allocates memory to store the file in a buffer
// Caller MUST free the memory once done using it.
// This function gets the file from disk
File_instance *get_resource(char string[], int *HTTP_code);

File_instance *get_file_size(char string[], int *HTTP_code);

// HTTP status is passed in to mark if a path traversal attempt was required
int transform_path(char string[], int *HTTP_code);

#endif