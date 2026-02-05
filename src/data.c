#include "data.h"

// For now, this file simulates accessing the HTML "file" from an actual file.
const char *html =
    "<!DOCTYPE html>"
    "<html>"
    "<head><title>C Server</title></head>"
    "<body><h1>Hello from C</h1></body>"
    "</html>"
;

const char *get_resource(const char string[]) {
    if (strcmp(string, "/") == 0) {
        return html;
    } else {
        return "";
    }
}