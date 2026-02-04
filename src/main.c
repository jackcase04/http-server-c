#include "processor.h"
#include "server.h"

int main(void) {

    server_connection http_server = setup_server();
    start_server(&http_server);

    return 0;
}