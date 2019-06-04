#pragma once

#include <arpa/inet.h>
#include <assert.h>
#include <netinet/ip.h>
#include <stddef.h>
#include <string.h>
#include <sys/socket.h>

#define guard_negone(x) if (x == -1) { return -1; }

int tcp_server(char*, uint16_t, int);

ssize_t tcp_send(int, char*);
