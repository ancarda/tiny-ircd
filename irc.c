#include <string.h>

const int IRC_MAX_NICK_LEN = 9;

struct IrcConn {
	int peer;
	char *nick;
};

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
    int  len;
    char *cmd = malloc(strlen(line));

    len = strcspn(line, " ");
    memcpy(cmd, line, len);

    if (strcmp(cmd, "NICK") == 0)
    {
        // TODO(ancarda): Check this against IRC_MAX_NICK_LEN.
        char *nick = malloc(strlen(line) - 5);

        // TODO(ancarda): Handle actual line endings.
        // Here, 2 is taken off the length to accomodate CrLf. Perhaps this
        // could be handled in a smarter way?
        memcpy(nick, line + 5, strlen(line) - 7);

        // TODO(ancarda): Can't memcpy write directly to irc->nick?
        // When I tried this, I just got a segfault. Not sure why just yet.
        irc->nick = nick;

        irc_notice(irc, "Nick changed!");
    }
    else if (strcmp(line, "WHOAMI\r\n") == 0)
    {
        irc_notice(irc, irc->nick);
    }
    else if (strcmp(cmd, "JOIN") == 0)
    {
        irc_notice(irc, "JOIN is not yet supported :(");
    }
    else
    {
        irc_notice(irc, "Unknown command");
    }
}
