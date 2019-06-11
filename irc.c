#include "tcp.h"
#include "irc.h"

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

int irc_pong(struct IrcConn* irc)
{
    char*  line;
    size_t line_len;
    int    status;
    char*  ping;

    ping = strtok(NULL, "\r\n");
    if (ping == NULL)
    {
        return 0;
    }

    line_len = 8 + strlen(ping);
    line = malloc(line_len);
    snprintf(line, line_len, "PONG %s\r\n", ping);

    status = tcp_send(irc->peer, line);
    free(line);
    return status;
}

int irc_rpl_001_welcome(struct IrcConn* irc)
{
    char*  line;
    size_t line_len;
    int    status;

    assert(irc->nick != NULL);

    line_len = 17 + strlen(irc->nick);
    line     = malloc(line_len);
    snprintf(line, line_len, "001 :Welcome %s\r\n", irc->nick);

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

int irc_err_433_nickname_in_use(struct IrcConn* irc)
{
    return tcp_send(irc->peer, "433 :Nickname in use\r\n");
}

char walk_nick_eq(struct IrcConn* irc, void* nick)
{
    if (irc->nick == NULL)
    {
        return IRCCONNPOOL_WALK_NEXT_ITEM;
    }

    return strcmp(irc->nick, (char*) nick) == 0;
}

struct walk_cond_notice_data
{
    char*           msg;
    struct IrcConn* excl;
};

char walk_cond_notice(struct IrcConn* irc, void* arg)
{
    struct walk_cond_notice_data* cn;

    cn = (struct walk_cond_notice_data*) arg;

    if (irc == cn->excl)
    {
        return IRCCONNPOOL_WALK_NEXT_ITEM;
    }

    irc_notice(irc, cn->msg);

    return IRCCONNPOOL_WALK_NEXT_ITEM;
}

void handle_nick(struct IrcConn* irc, IrcConnPool* pool)
{
    char* nick;
    char  was_null;

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

    if (ircconnpool_walk(pool, walk_nick_eq, nick) != NULL)
    {
        irc_err_433_nickname_in_use(irc);
        return;
    }

    was_null = (irc->nick == NULL) ? 1 : 0;

    FREE_NOT_NULL(irc->nick);
    irc->nick = malloc(strlen(nick) + 1);
    strcpy(irc->nick, nick);
    irc_notice(irc, "Nick changed!");

    if (was_null == 1)
    {
        irc_rpl_001_welcome(irc);

        int broadcast_msg_len = 34 + strlen(nick);
        char* broadcast_msg = malloc(broadcast_msg_len);
        struct walk_cond_notice_data cn;

        snprintf(broadcast_msg, broadcast_msg_len, "Say hello to our newest member: %s!", nick);
        cn.msg = broadcast_msg;
        cn.excl = irc;
        ircconnpool_walk(pool, walk_cond_notice, &cn);

        free(broadcast_msg);
    }
}

void handle_join(struct IrcConn* irc, ChanPool* cp, char* chan_name)
{
    // TODO(ancarda): chanpool_walk to find the requested channel.
    // If it does not exist, create it and push it into the ChanPool.
    // Finally, register the user bidirectionally:
    //
    //     ircconn_push(chan);
    //     chan_push(irc);
}

void handle_irc_packet(struct IrcConn* irc, IrcConnPool* pool, ChanPool* cp, char* line)
{
    char* cmd;

    cmd = strtok(line, " ");
    if (strcmp(cmd, "NICK") == 0)
    {
        handle_nick(irc, pool);
    }
    else if (strcmp(cmd, "PING") == 0)
    {
        irc_pong(irc);
    }
    else
    {
        // TODO(ancarda): Replace this with error 421 (Unknown Command)
        irc_notice(irc, "Unknown command");
    }
}
