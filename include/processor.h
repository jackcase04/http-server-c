#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdio.h>
#include <string.h>
#include "types.h"
#include "data.h"

void process_request(Request *request);

void split(const char input[], char output[][256], char delimeter);

#endif