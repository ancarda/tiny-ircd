#include "chan.h"

typedef struct ChanEntry ChanEntry;

struct ChanEntry
{
    struct IrcConn* val;
    ChanEntry*      nxt;
};

ChanEntry* chanentry_make(struct IrcConn* irc)
{
    ChanEntry* ce;

    ce = malloc(sizeof(*ce));
    assert(ce != NULL);
    ce->val = irc;
    ce->nxt = NULL;

    return ce;
}

struct Chan
{
    char*      name;
    char*      topic;  /*@NULL@*/
    ChanEntry* p_head; /*@NULL@*/
    ChanEntry* p_tail; /*@NULL@*/
    int        len;
};

Chan* chan_make(char* name)
{
    Chan* chan;

    chan = malloc(sizeof(*chan));
    assert(chan != NULL);

    chan->name   = strdup(name);
    chan->topic  = NULL;
    chan->p_head = NULL;
    chan->p_tail = NULL;
    chan->len    = 0;

    return chan;
}

char* chan_getname(Chan* chan)
{
    return strdup(chan->name);
}

char* chan_gettopic(Chan* chan)
{
    if (chan->topic == NULL)
    {
        return NULL;
    }

    return strdup(chan->topic);
}

void chan_settopic(Chan* chan, char* topic)
{
    size_t len;

    FREE_NOT_NULL(chan->topic);

    if (topic == NULL)
    {
        chan->topic = NULL;
        return;
    }

    // TODO(ancarda): Allow a limit on chan length
    len = strlen(topic);
    chan->topic = malloc(sizeof(char) * len + 1);
    assert(chan->topic != NULL);

    strcpy(chan->topic, topic);
}

int chan_len(Chan* chan)
{
    return chan->len;
}

void chan_push(Chan* chan, struct IrcConn* irc)
{
    ChanEntry* ce;

    ce = chanentry_make(irc);
    chan->len++;

    if (chan->p_head == NULL)
    {
        chan->p_head = ce;
        chan->p_tail = chan->p_head;
        return;
    }

    chan->p_tail->nxt = ce;
    chan->p_tail = ce;
}
