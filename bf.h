#ifndef __BIGFLOAT_H__
#define __BIGFLOAT_H__

#include "bn_s.h"

//#define BF_NORMALIZATION_MANTISSA_SIZE(x) ((BN_BYTES * x) -1)

#define BF_BASE 2

#define BN_PREFIX
#define BN_ARRAY_SIZE_MOD 1
#include "bf_.h"
#undef BN_PREFIX
#undef BN_ARRAY_SIZE_MOD
#define BN_PREFIX _2
#define BN_ARRAY_SIZE_MOD 2
#include "bf_.h"
#undef BN_PREFIX
#undef BN_ARRAY_SIZE_MOD

#endif
