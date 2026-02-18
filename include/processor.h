#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdio.h>
#include <string.h>
#include "types.h"
#include "data.h"

void process_request(server_connection *server, const char request[]);

void decide_response(server_connection *server, char tokens[][256]);

void split(const char input[], char output[][256], char delimeter);

#endif