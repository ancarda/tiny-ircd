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

void handle_nick(struct IrcConn* irc, struct IrcConnPool* pool)
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

    // TODO(ancarda): Replace traversal with some generic walk function
    //
    // Something like ircconnpool_walk(pool, fn, arg) might be useful, where
    // the given callback looks like this:
    //
    //     char fn(struct IrcConn item*, void* arg)
    //
    // If fn() returns 1, it means move to the next item. If 0 is returned,
    // it would mean return `item` to the parent. NULL is given if fn() never
    // returned 1.
    //
    // An advantage is walk() can place locks around the pool. Of course, I
    // could lock here, but locking and - touching len - outside ircconnpool.c
    // makes me uncomfortable. I'd rather centralize what will be a very common
    // operation (loop over the pool with some basic predicate) into a well
    // tested function.
    for (int i = 0; i < pool->len; i++)
    {
        if (pool->val[i]->nick != NULL && strcmp(pool->val[i]->nick, nick) == 0)
        {
            irc_err_433_nickname_in_use(irc);
            return;
        }
    }

    was_null = (irc->nick == NULL) ? 1 : 0;

    free_notnull(irc->nick);
    irc->nick = malloc(strlen(nick));
    strcpy(irc->nick, nick);
    irc_notice(irc, "Nick changed!");

    if (was_null == 1)
    {
        int broadcast_msg_len = 33 + strlen(nick);
        char* broadcast_msg = malloc(broadcast_msg_len);
        snprintf(broadcast_msg, broadcast_msg_len, "Say hello to our newest member: %s!", nick);
        for (int i = 0; i < pool->len; i++)
        {
            if (pool->val[i] != irc)
            {
                irc_notice(pool->val[i], broadcast_msg);
            }
        }

        free(broadcast_msg);
    }
}

void handle_irc_packet(struct IrcConn* irc, struct IrcConnPool* pool, char* line)
{
    char* cmd;

    cmd = strtok(line, " ");
    if (strcmp(cmd, "NICK") == 0)
    {
        handle_nick(irc, pool);
    }
    else
    {
        irc_notice(irc, "Unknown command");
    }
}
