#pragma once

#include <pthread.h>
#include "tinyirc.h"
#include "chan.h"

typedef struct ChanPool ChanPool;

ChanPool* chanpool_make();

void chanpool_push(ChanPool*, Chan*);

int chanpool_len(ChanPool*);

Chan* chanpool_find_by_name(char* name);

// Walk the pool, calling the provided function (second argument):
//
//     fn(Chan*, arg) -> char
//
// The 3rd argument of walk() is passed to the callback (listed above as arg).
//
// If fn() returns WALK_NEXT_ITEM, walk will move to the next item.
// If fn() returns WALK_TERMINATE, walk will return a value to the caller.
// Other return values are technically undefined.
// If the pool is exhausted without fn() returning 0, walk returns NULL.
//
// The last argument is the mode, which must be one of:
//
//     WALK_MODE_RETURN_ARG - Returns the given argument
//     WALK_MODE_RETURN_VAL - Returns the found object
//
// Passing an incorrect mode will cause a panic (assertion).
void* chanpool_walk(ChanPool*, char (Chan*, void*), void*, int);
