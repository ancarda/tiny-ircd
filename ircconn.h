#pragma once

#include <stdio.h>
#include <stdlib.h>

struct IrcConn {
    int   peer;
    char* nick;
};

struct IrcConn* ircconn_make();

void ircconn_free(struct IrcConn* irc);
