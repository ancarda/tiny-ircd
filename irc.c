#include "tcp.h"
#include "irc.h"

void irc_notice(struct IrcConn *irc, char *msg)
{
    // TODO(ancarda): Stop being lazy and actually do string concatenation.
    // TODO(ancarda): Handle tcp_send errors.
    tcp_send(irc->peer, "NOTICE ");
    if (irc->nick == NULL)
    {
        tcp_send(irc->peer, "anonymous");
    }
    else
    {
        tcp_send(irc->peer, irc->nick);
    }
    tcp_send(irc->peer, " :");
    if (msg != NULL) {
        tcp_send(irc->peer, msg);
    }
    tcp_send(irc->peer, "\r\n");
}

void handle_irc_packet(struct IrcConn *irc, char *line)
{
    char *cmd;

    cmd = strtok(line, " ");
    if (strcmp(cmd, "NICK") == 0)
    {
        char *nick;

        if (irc->nick != NULL)
        {
            free(irc->nick);
        }

        nick = strtok(NULL, "\r\n");
        irc->nick = malloc(strlen(nick));
        strcpy(irc->nick, nick);
        irc_notice(irc, "Nick changed!");
    }
    else
    {
        irc_notice(irc, "Unknown command");
    }
}
