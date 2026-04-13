/*
 * servicemesh
 * ServiceMesh: Lightweight service mesh with load balancing and circuit breaking
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "server.h"
#include "database.h"

static volatile int running = 1;

static void handle_signal(int sig) {
    (void)sig;
    running = 0;
}

int main(int argc, char *argv[]) {
    int port = 8080;
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    signal(SIGINT,  handle_signal);
    signal(SIGTERM, handle_signal);

    printf("servicemesh starting on port %d\n", port);

    if (db_init("app.db") != 0) {
        fprintf(stderr, "Failed to initialize database\n");
        return EXIT_FAILURE;
    }

    server_t *srv = server_create(port);
    if (!srv) {
        fprintf(stderr, "Failed to create server\n");
        db_close();
        return EXIT_FAILURE;
    }

    printf("Server ready. Press Ctrl+C to stop.\n");

    while (running) {
        server_poll(srv, 100);
    }

    server_destroy(srv);
    db_close();
    printf("Shutdown complete.\n");
    return EXIT_SUCCESS;
}
