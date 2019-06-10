#include <assert.h>
#include <string.h>
#include "tinyirc.h"
#include "ircconn.h"

struct Chan;

struct Chan* chan_make();

// Returns a copy of this channel's name, eg. #lobby
//
// Returned pointer must be freed by the upstream caller.
char* chan_getname(struct Chan*);

// Returns a copy of this channel's topic, eg. Default channel
//
// Returned value may be NULL.
//
// Returned pointer must be freed by the upstream caller.
char* chan_gettopic(struct Chan*);

// Sets this channel's topic.
void chan_settopic(struct Chan*, char*);

// Returns a count of the people in this channel.
int chan_len(struct Chan*);

// Adds a new irc connection into this channel.
void chan_push(struct Chan*, struct IrcConn*);
