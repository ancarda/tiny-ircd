#pragma once

#include <assert.h>
#include "ircconn.h"

struct IrcConnPool
{
    int              len;
    int              cap;
    struct IrcConn** val;
};

struct IrcConnPool* ircconnpool_make(int cap);

void ircconnpool_free(struct IrcConnPool* pool);

void ircconnpool_push(struct IrcConnPool* pool, struct IrcConn* conn);
