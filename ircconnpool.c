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
