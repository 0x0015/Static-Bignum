#ifndef __BIGFLOAT_H__
#define __BIGFLOAT_H__

#include "bn_s.h"

//#define BF_NORMALIZATION_MANTISSA_SIZE(x) ((BN_BYTES * x) -1)

#define BF_BASE 2

//TODO:  used double precision to increase multiplication accuracy

#define BN_PREFIX
#define BN_ARRAY_SIZE_MOD 1
#include "bf_.h"
#undef BN_PREFIX
#undef BN_ARRAY_SIZE_MOD

#ifndef BN_NO_DOUBLE_P
#define BN_PREFIX _2
#define BN_ARRAY_SIZE_MOD 2
#include "bf_.h"
#undef BN_PREFIX
#undef BN_ARRAY_SIZE_MOD

void bigfloat_to_2(BN_VAR_PREFIX struct bf_2* dst, BN_VAR_PREFIX struct bf* src);
void bigfloat_2_to_1(BN_VAR_PREFIX struct bf* dst, BN_VAR_PREFIX struct bf_2* src);
#endif

#endif
