#include "chanpool.h"

typedef struct ChanPoolEntry ChanPoolEntry;

struct ChanPoolEntry
{
    Chan*          val;
    ChanPoolEntry* nxt;
};

struct ChanPool
{
    ChanPoolEntry* p_head; /*@NULL@*/
    ChanPoolEntry* p_tail; /*@NULL@*/
    int            len;
    int            cap;
};

ChanPoolEntry* chanpoolentry_make(Chan* chan)
{
    ChanPoolEntry* cpe;

    cpe = malloc(sizeof(*cpe));
    assert(cpe != NULL);
    cpe->val = chan;
    cpe->nxt = NULL;

    return cpe;
}

ChanPool* chanpool_make()
{
    ChanPool* cp;

    cp = malloc(sizeof(*cp));
    assert(cp != NULL);

    cp->p_head = NULL;
    cp->p_tail = NULL;
    cp->len    = 0;
    cp->cap    = 10;

    return cp;
}

void chanpool_push(ChanPool* cp, Chan* chan)
{
    ChanPoolEntry* cpe;

    cpe = chanpoolentry_make(chan);
    cp->len++;

    if (cp->p_head == NULL)
    {
        cp->p_head = cpe;
        cp->p_tail = cp->p_head;
        return;
    }

    cp->p_tail->nxt = cpe;
    cp->p_tail = cpe;
}

int chanpool_len(ChanPool* cp)
{
    return cp->len;
}
