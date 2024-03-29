#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include "tcp.h"
#include "ircconn.h"
#include "ircconnpool.h"
#include "irc.h"
#include "chan.h"
#include "chanpool.h"

#define LISTEN_BACKLOG 10

struct Context
{
    IrcConnPool* pool;
    ChanPool*    cp;
    int          peer;
};

void *handle_client(void* arg)
{
    char            buf[64];
    struct Context* ctx;
    struct IrcConn* irc;

    ctx = (struct Context*) arg;

    irc = ircconn_make();
    irc->peer = ctx->peer;

    ircconnpool_push(ctx->pool, irc);

    irc_notice(irc, "tiny-ircd");

    while (1)
    {
        if (read(irc->peer, &buf, sizeof(buf)) <= 0)
        {
            perror("read()");
            goto cleanup;
        }

        handle_irc_packet(irc, ctx->pool, ctx->cp, buf);
    }

cleanup:
    shutdown(irc->peer, SHUT_RDWR);

    ircconn_free(irc);
    ircconnpool_remove(ctx->pool, irc);

    free(ctx);

    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t*          threads;
    int                 threads_len;
    int                 threads_cap;
    int                 sock;
    int                 peer;
    struct sockaddr     peer_addr;
    socklen_t           peer_addr_sizeof;
    IrcConnPool*        pool;
    struct Context*     ctx;
    ChanPool*           cp;

    pool = ircconnpool_make(10);
    cp = chanpool_make();
    threads_len = 0;
    threads_cap = 100;
    threads = malloc(threads_cap * sizeof(pthread_t));

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

        if (threads_len == threads_cap)
        {
            threads = realloc(threads, (threads_len + 100) * sizeof(pthread_t));
            threads_cap = threads_len + 100;
        }

        ctx = malloc(sizeof(*ctx));
        ctx->pool = pool;
        ctx->cp   = cp;
        ctx->peer = peer;

        pthread_create(&threads[threads_len], NULL, handle_client, ctx);
        threads_len++;
    }

    return EXIT_SUCCESS;
}
