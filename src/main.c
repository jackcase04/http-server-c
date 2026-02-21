#include <stdlib.h>

#include "processor.h"
#include "server.h"

char WEB_ROOT[PATH_MAX];

int main(void) {

    realpath("public", WEB_ROOT);

    Server http_server = setup_server();
    start_server(&http_server);

    return 0;
}