#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IRC_MAX_NICK_LEN 9
#define IRC_PORT         6667

#define free_notnull(x) if (x != NULL) { free(x); }

struct IrcConn {
    int   peer;
    char* nick;
};

void ircconn_free(struct IrcConn* irc);

int irc_notice(struct IrcConn*, char*);

void handle_irc_packet(struct IrcConn*, char*);
