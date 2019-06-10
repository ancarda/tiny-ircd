#include <pthread.h>
#include "chan.h"

typedef struct ChanPool ChanPool;

ChanPool* chanpool_make();

void chanpool_push(ChanPool*, Chan*);

int chanpool_len(ChanPool*);
