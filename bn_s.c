#include "bn_s.h"

#define BN_PREFIX
#define BN_ARRAY_SIZE_MOD 1
#include "bn_s_.c"
#undef BN_PREFIX
#undef BN_ARRAY_SIZE_MOD
#define BN_PREFIX _2
#define BN_ARRAY_SIZE_MOD 2
#include "bn_s_.c"
#undef BN_PREFIX
#undef BN_ARRAY_SIZE_MOD

