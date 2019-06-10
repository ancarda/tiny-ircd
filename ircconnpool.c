#include "ircconnpool.h"

struct IrcConnPool
{
    int              len;
    int              cap;
    struct IrcConn** val;
    pthread_mutex_t* lck;
};

void __lock(struct IrcConnPool* pool)
{
    assert(pthread_mutex_lock(pool->lck) == 0);
}

void __unlock(struct IrcConnPool* pool)
{
    assert(pthread_mutex_unlock(pool->lck) == 0);
}

struct IrcConnPool* ircconnpool_make(size_t cap)
{
    struct IrcConnPool* pool;
    pthread_mutex_t*    mtx;

    assert(cap > 0);

    mtx  = malloc(sizeof(*mtx));
    pool = malloc(sizeof(*pool));
    pool->len = 0;
    pool->cap = cap;
    pool->val = malloc(pool->cap * sizeof(pool->val));

    assert(pthread_mutex_init(mtx, NULL) == 0);
    pool->lck = mtx;

    return pool;
}

void ircconnpool_free(struct IrcConnPool* pool)
{
    int i;
    for (i = 0; i <= pool->len; i++)
    {
        ircconn_free(pool->val[i]);
    }

    pthread_mutex_destroy(pool->lck);
    free(pool->lck);
    free(pool);
}

void ircconnpool_push(struct IrcConnPool* pool, struct IrcConn* conn)
{
    __lock(pool);

    if (pool->len == pool->cap)
    {
        pool->val = realloc(pool->val, (pool->len + 5) * (sizeof(pool->val)));
        pool->cap = pool->len + 5;
    }

    pool->val[pool->len] = conn;
    pool->len++;

    __unlock(pool);
}

bool ircconnpool_remove(struct IrcConnPool* pool, struct IrcConn* conn)
{
    int i;

    __lock(pool);

    for (i = 0; i < pool->len; i++)
    {
        if (pool->val[i] == conn)
        {
            goto destroy;
        }
    }

    __unlock(pool);

    return false;

destroy:
    for (; i < pool->len; i++)
    {
        pool->val[i] = pool->val[i + 1];
    }
    pool->len--;

    __unlock(pool);

    return true;
}

int ircconnpool_len(struct IrcConnPool* pool)
{
    return pool->len;
}

void* ircconnpool_walk(struct IrcConnPool* pool, char (fn)(struct IrcConn*, void*), void* arg)
{
    int  i;
    char r;

    __lock(pool);

    for (i = 0; i < pool->len; i++)
    {
        r = fn(pool->val[i], arg);

        if (r == 1)
        {
            __unlock(pool);
            return arg;
        }
    }

    __unlock(pool);
    return NULL;
}
