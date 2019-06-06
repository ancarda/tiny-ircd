#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ircconn.h"

#define IRC_MAX_NICK_LEN 9
#define IRC_PORT         6667

#define free_notnull(x) if (x != NULL) { free(x); }

int irc_notice(struct IrcConn*, char*);

int irc_err_431_no_nick_given(struct IrcConn* irc);

int irc_err_432_erroneous_nickname(struct IrcConn* irc, char* given_msg);

void handle_irc_packet(struct IrcConn*, char*);
