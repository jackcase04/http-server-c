#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This function enforces the files that the users can access
int check_resource_exists(const char string[]);

// This function actually gets the file from disk
int get_resource(char string[], char file_buff[], int len);

size_t get_file_size(char string[]);

int transform_path(char string[]);

#endif