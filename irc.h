#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ircconn.h"
#include "ircconnpool.h"

#define IRC_MAX_NICK_LEN 9
#define IRC_PORT         6667

#define free_notnull(x) if (x != NULL) { free(x); }

int irc_notice(struct IrcConn*, char*);

int irc_err_431_no_nick_given(struct IrcConn*);

int irc_err_432_erroneous_nickname(struct IrcConn*, char*);

int irc_err_433_nickname_in_use(struct IrcConn* irc);

void handle_nick(struct IrcConn* irc, struct IrcConnPool* pool);

void handle_irc_packet(struct IrcConn*, struct IrcConnPool*, char*);
