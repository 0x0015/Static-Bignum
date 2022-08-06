#ifndef __BIGNUM_SIGNED_H__
#define __BIGNUM_SIGNED_H__

#include "bn.h"


#define BN_PREFIX
#define BN_ARRAY_SIZE_MOD 1
#include "bn_s_.h"
#undef BN_PREFIX
#undef BN_ARRAY_SIZE_MOD
#define BN_PREFIX _2
#define BN_ARRAY_SIZE_MOD 2
#include "bn_s_.h"
#undef BN_PREFIX
#undef BN_ARRAY_SIZE_MOD

void bignum_signed_to_2(BN_VAR_PREFIX struct bn_s_2* dst, BN_VAR_PREFIX struct bn_s* src);
void bignum_signed_2_to_1(BN_VAR_PREFIX struct bn_s* dst, BN_VAR_PREFIX struct bn_s_2* src);

#endif
