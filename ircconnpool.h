#pragma once

#include <assert.h>
#include <pthread.h>
#include "ircconn.h"

#define IRCCONNPOOL_WALK_NEXT_ITEM  0
#define IRCCONNPOOL_WALK_RETURN_ARG 1

struct IrcConnPool;

struct IrcConnPool* ircconnpool_make(int);

void ircconnpool_free(struct IrcConnPool*);

void ircconnpool_push(struct IrcConnPool*, struct IrcConn*);

int ircconnpool_remove(struct IrcConnPool*, struct IrcConn*);

// Walk the pool, calling the provided function (second argument):
//
//     fn(struct IrcConn*, arg) -> char
//
// The 3rd argument of walk() is passed to the callback (listed above as arg).
//
// If fn() returns 1, walk will return arg to the upstream caller.
// If fn() returns 0, walk will move to the next item.
// If the pool is exhausted without fn() returning 0, walk returns NULL.
void* ircconnpool_walk(struct IrcConnPool*, char (struct IrcConn*, void*), void*);
