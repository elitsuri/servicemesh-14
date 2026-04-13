/* server.h — servicemesh server interface */
#pragma once

typedef struct server server_t;

server_t *server_create(int port);
void      server_poll(server_t *s, int timeout_ms);
void      server_destroy(server_t *s);
