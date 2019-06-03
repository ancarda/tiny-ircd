#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include "tcp.h"
#include "irc.h"

#define graceful_negone(x, fn) if (x == -1) { perror(fn); goto cleanup; }

const int IRC_PORT       = 6667;
const int LISTEN_BACKLOG = 10;

void *handle_client(void *args)
{
	int            status;
	char           buf[64];
	ssize_t        len;
	struct IrcConn irc;

	irc.peer = *((int *) args);
	irc.nick = malloc(IRC_MAX_NICK_LEN + 1);

	status = tcp_send(irc.peer, "NOTICE anonymous: tiny-ircd.\r\n");
	graceful_negone(status, "send()");

	while (1)
	{
		len = read(irc.peer, &buf, sizeof(buf));
		graceful_negone(len, "read()");

		handle_irc_packet(&irc, buf);
	}

	cleanup:
		shutdown(irc.peer, SHUT_RDWR);

	return NULL;
}

int main(int argc, char *argv[])
{
	int       sock;
	int       peer;
	struct    sockaddr peer_addr;
	socklen_t peer_addr_sizeof;
	pthread_t thread_id;

	sock = tcp_server("127.0.0.1", IRC_PORT, LISTEN_BACKLOG);
	if (sock == -1)
	{
		perror("tcp_server()");
		exit(EXIT_FAILURE);
	}

	printf("Listening on 127.0.0.1:%d\n", IRC_PORT);

	while (1)
	{
		peer_addr_sizeof = sizeof(peer_addr);
		peer = accept(sock, &peer_addr, &peer_addr_sizeof);
		if (sock == -1)
		{
			perror("accept()");
			exit(EXIT_FAILURE);
		}

		pthread_create(&thread_id, NULL, handle_client, &peer);
	}

	return EXIT_SUCCESS;
}
