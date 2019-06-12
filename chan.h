#pragma once

#include <assert.h>
#include <string.h>
#include "tinyirc.h"
#include "ircconn.h"

typedef struct Chan Chan;

Chan* chan_make();

// Returns a copy of this channel's name, eg. #lobby
//
// Returned pointer must be freed by the upstream caller.
char* chan_getname(Chan*);

// Returns a copy of this channel's topic, eg. Default channel
//
// Returned value may be NULL.
//
// Returned pointer must be freed by the upstream caller.
char* chan_gettopic(Chan*);

// Sets this channel's topic.
void chan_settopic(Chan*, char*);

// Returns a count of the people in this channel.
int chan_len(Chan*);

// Adds a new irc connection into this channel.
void chan_push(Chan*, struct IrcConn*);

// Walk function that finds a channel based on it's name.
char chan_walkfn_name_eq(Chan*, void*);
