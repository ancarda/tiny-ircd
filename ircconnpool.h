#pragma once

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include "ircconn.h"

#define IRCCONNPOOL_WALK_NEXT_ITEM  0
#define IRCCONNPOOL_WALK_RETURN_ARG 1

typedef struct IrcConnPool IrcConnPool;

IrcConnPool* ircconnpool_make(size_t);

void ircconnpool_free(IrcConnPool*);

void ircconnpool_push(IrcConnPool*, struct IrcConn*);

bool ircconnpool_remove(IrcConnPool*, struct IrcConn*);

int ircconnpool_len(IrcConnPool*);

// Walk the pool, calling the provided function (second argument):
//
//     fn(struct IrcConn*, arg) -> char
//
// The 3rd argument of walk() is passed to the callback (listed above as arg).
//
// If fn() returns 1, walk will return arg to the upstream caller.
// If fn() returns 0, walk will move to the next item.
// If the pool is exhausted without fn() returning 0, walk returns NULL.
void* ircconnpool_walk(IrcConnPool*, char (struct IrcConn*, void*), void*);
