#include "tcp.h"
#include "irc.h"

void ircconn_free(struct IrcConn* irc)
{
    free_notnull(irc->nick);
    free(irc);
}

int irc_notice(struct IrcConn* irc, char* msg)
{
    char*  nick;
    char*  line;
    size_t line_len;
    int    status;

    nick = (irc->nick == NULL) ? "anonymous" : irc->nick;

    // Allocate bytes for: "NOTICE" (7), the user's nick, " :" (2), the actual
    // message, "\r\n\0" (3).
    line_len = 7 + strlen(nick) + 2 + strlen(msg) + 3;

    line = malloc(line_len);
    snprintf(line, line_len, "NOTICE %s :%s\r\n", nick, msg);
    status = tcp_send(irc->peer, line);

    free(line);

    return status;
}

void handle_irc_packet(struct IrcConn* irc, char* line)
{
    char* cmd;

    cmd = strtok(line, " ");
    if (strcmp(cmd, "NICK") == 0)
    {
        char* nick;

        free_notnull(irc->nick);
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
