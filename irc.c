#include "tcp.h"
#include "irc.h"

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

int irc_err_431_no_nick_given(struct IrcConn* irc)
{
	return tcp_send(irc->peer, "431 :No nickname given\r\n");
}

int irc_err_432_erroneous_nickname(struct IrcConn* irc, char* given_msg)
{
    char*  nick;
    char*  msg;
    char*  line;
    size_t line_len;
    int    status;

    nick = (irc->nick == NULL) ? "anonymous" : irc->nick;
    if (given_msg == NULL)
    {
        msg = "Erroneous nickname";
    }
    else
    {
        msg = given_msg;
    }

    // Allocate bytes for: "432 " (4), the user's nick, " :" (2), the actual
    // message, and characters to end the packet -- "\r\n\0" (3).
    line_len = 4 + strlen(nick) + 2 + strlen(msg) + 3;

    line = malloc(line_len);
    snprintf(line, line_len, "432 %s :%s\r\n", nick, msg);
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

        nick = strtok(NULL, "\r\n");
        if (nick == NULL)
        {
            irc_err_431_no_nick_given(irc);
            return;
        }

        if (strlen(nick) > IRC_MAX_NICK_LEN)
        {
            irc_err_432_erroneous_nickname(irc, "Nick exceeds 9 characters");
            return;
        }

        // TODO(ancarda): Support ERR_NICKNAMEINUSE (433)

        free_notnull(irc->nick);
        irc->nick = malloc(strlen(nick));
        strcpy(irc->nick, nick);
        irc_notice(irc, "Nick changed!");
    }
    else
    {
        irc_notice(irc, "Unknown command");
    }
}
