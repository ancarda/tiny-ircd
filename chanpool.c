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

void* chanpool_walk(ChanPool* pool, char (fn)(Chan*, void*), void* arg, int mode)
{
    struct ChanPoolEntry* ptr;
    char                  ret;

    assert(mode == WALK_MODE_RETURN_ARG || mode == WALK_MODE_RETURN_VAL);

    // TODO(ancarda): lock

    ptr = pool->p_head;

    while (ptr != NULL)
    {
        ret = fn(ptr->val, arg);
        if (ret == WALK_TERMINATE)
        {
            // TODO(ancarda): unlock

            if (mode == WALK_MODE_RETURN_ARG)
            {
                return arg;
            }
            else if (mode == WALK_MODE_RETURN_VAL)
            {
                return ptr->val;
            }
        }

        ptr = ptr->nxt;
    }

    // TODO(ancarda): unlock

    return NULL;
}
