#include "ircconn.h"

struct IrcConn* ircconn_make()
{
    struct IrcConn* irc;

    irc = malloc(sizeof(struct IrcConn*));
    irc->peer = 0;
    irc->nick = NULL;

    return irc;
}

void ircconn_free(struct IrcConn* irc)
{
    free_notnull(irc->nick);
    free(irc);
}
