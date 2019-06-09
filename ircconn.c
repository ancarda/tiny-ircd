#include "tinyirc.h"
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
    FREE_NOT_NULL(irc->nick);
    free(irc);
}
