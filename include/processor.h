#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdio.h>
#include <string.h>
#include "types.h"

void process_request(server_connection *server, const char request[]);

void decide_response(char tokens[][256], char message[], int *message_len);

void split(const char input[], char output[][256], char delimeter);

#endif