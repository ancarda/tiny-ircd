#include "chan.h"

struct ChanEntry
{
    struct IrcConn*   val;
    struct ChanEntry* nxt;
};

struct ChanEntry* chanentry_make(struct IrcConn* irc)
{
    struct ChanEntry* ce;

    ce = malloc(sizeof(*ce));
    assert(ce != NULL);
    ce->val = irc;
    ce->nxt = NULL;

    return ce;
}

struct Chan
{
    char*             name;
    char*             topic;  /*@NULL@*/
    struct ChanEntry* p_head; /*@NULL@*/
    struct ChanEntry* p_tail; /*@NULL@*/
    int               len;
};

struct Chan* chan_make(char* name)
{
    struct Chan* chan;

    chan = malloc(sizeof(*chan));
    assert(chan != NULL);

    chan->name   = strdup(name);
    chan->topic  = NULL;
    chan->p_head = NULL;
    chan->p_tail = NULL;
    chan->len    = 0;

    return chan;
}

char* chan_getname(struct Chan* chan)
{
    return strdup(chan->name);
}

char* chan_gettopic(struct Chan* chan)
{
    if (chan->topic == NULL)
    {
        return NULL;
    }

    return strdup(chan->topic);
}

void chan_settopic(struct Chan* chan, char* topic)
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

int chan_len(struct Chan* chan)
{
    return chan->len;
}

void chan_push(struct Chan* chan, struct IrcConn* irc)
{
    struct ChanEntry* ce;

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
