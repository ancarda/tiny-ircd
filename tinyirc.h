#pragma once

#define TINYIRC_VERSION "0.0.0-dev"
#define FREE_NOT_NULL(x) if (x != NULL) { free(x); }

#define WALK_NEXT_ITEM 0
#define WALK_TERMINATE 1

#define WALK_MODE_RETURN_ARG 0
#define WALK_MODE_RETURN_VAL 1
