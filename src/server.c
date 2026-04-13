/*
 * server.c — TCP/HTTP server implementation for servicemesh
 */
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct server {
    int  sockfd;
    int  port;
};

server_t *server_create(int port) {
    server_t *s = malloc(sizeof(*s));
    if (!s) return NULL;

    s->port   = port;
    s->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (s->sockfd < 0) { free(s); return NULL; }

    int opt = 1;
    setsockopt(s->sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {0};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons((uint16_t)port);

    if (bind(s->sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(s->sockfd); free(s); return NULL;
    }
    if (listen(s->sockfd, 10) < 0) {
        close(s->sockfd); free(s); return NULL;
    }
    return s;
}

void server_poll(server_t *s, int timeout_ms) {
    struct timeval tv = { timeout_ms / 1000, (timeout_ms % 1000) * 1000 };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(s->sockfd, &fds);

    int ready = select(s->sockfd + 1, &fds, NULL, NULL, &tv);
    if (ready <= 0) return;

    int client = accept(s->sockfd, NULL, NULL);
    if (client < 0) return;

    char buf[4096] = {0};
    ssize_t n = recv(client, buf, sizeof(buf) - 1, 0);
    if (n > 0) {
        const char *resp =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Connection: close\r\n\r\n"
            "{\"status\":\"ok\",\"service\":\"servicemesh\"}\n";
        send(client, resp, strlen(resp), 0);
    }
    close(client);
}

void server_destroy(server_t *s) {
    if (s) { close(s->sockfd); free(s); }
}
