#pragma once

#include <assert.h>
#include "ircconn.h"

struct IrcConnPool
{
    int              len;
    int              cap;
    struct IrcConn** val;
};

struct IrcConnPool* ircconnpool_make(int);

void ircconnpool_free(struct IrcConnPool*);

void ircconnpool_push(struct IrcConnPool*, struct IrcConn*);

int ircconnpool_remove(struct IrcConnPool*, struct IrcConn*);
