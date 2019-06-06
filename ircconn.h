#pragma once

#include <stdio.h>
#include <stdlib.h>

#define free_notnull(x) if (x != NULL) { free(x); }

struct IrcConn {
    int   peer;
    char* nick;
};

struct IrcConn* ircconn_make();

void ircconn_free(struct IrcConn* irc);
