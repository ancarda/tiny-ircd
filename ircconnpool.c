#include "ircconnpool.h"

struct IrcConnPool* ircconnpool_make(int cap)
{
    struct IrcConnPool* pool;

    assert(cap > 0);

    pool = malloc(sizeof(pool));
    pool->len = 0;
    pool->cap = 10;
    pool->val = malloc(pool->cap * sizeof(pool->val));

    return pool;
}

void ircconnpool_free(struct IrcConnPool* pool)
{
    int i;
    for (i = 0; i <= pool->len; i++)
    {
        ircconn_free(pool->val[i]);
    }

    free(pool);
}

void ircconnpool_push(struct IrcConnPool* pool, struct IrcConn* conn)
{
    // TODO(ancarda): acquire mutex lock
    if (pool->len == pool->cap)
    {
        pool->val = realloc(pool->val, (pool->len + 5) * (sizeof(pool->val)));
        pool->cap = pool->len + 5;
    }

    pool->val[pool->len] = conn;
    pool->len++;
    // TODO(ancarda): release mutex lock
}

int ircconnpool_remove(struct IrcConnPool* pool, struct IrcConn* conn)
{
    int i;

    // TODO(ancarda): acquire mutex lock

    for (i = 0; i < pool->len; i++)
    {
        if (pool->val[i] == conn)
        {
            goto destroy;
        }
    }

    // TODO(ancarda): release mutex lock

    return 0;

destroy:
    for (; i < pool->len; i++)
    {
        pool->val[i] = pool->val[i + 1];
    }
    pool->len--;

    // TODO(ancarda): release mutex lock

    return 1;
}
