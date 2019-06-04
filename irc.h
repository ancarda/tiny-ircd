#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IRC_MAX_NICK_LEN 9

struct IrcConn {
    int   peer;
    char* nick;
};

void irc_notice(struct IrcConn*, char*);

void handle_irc_packet(struct IrcConn*, char*);
