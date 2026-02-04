#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdio.h>
#include <string.h>
#include "types.h"

void process_request(server_connection *server, const char request[]);

void split(const char input[], int len, char output[][256], char delimeter);

#endif