#ifndef __BIGNUM_SIGNED_H__
#define __BIGNUM_SIGNED_H__

#include "bn.h"

typedef struct bn_s{
	uint8_t sign;
	struct bn value;
}bn_s;

#define bn_s_zero ((bn_s){0,bn_zero})

void bignum_signed_init(BN_VAR_PREFIX struct bn_s* n);
void bignum_signed_from_bn(BN_VAR_PREFIX struct bn_s* n, BN_VAR_PREFIX struct bn* bigint);
void bignum_signed_from_int(BN_VAR_PREFIX struct bn_s* n, int64_t value);
int32_t bignum_signed_to_int(BN_VAR_PREFIX struct bn_s* n);

#ifndef BN_NO_STDIO
void bignum_signed_to_string(BN_VAR_PREFIX struct bn_s* n, char* str, int maxsize);
#endif

void bignum_signed_add(BN_VAR_PREFIX struct bn_s* a, BN_VAR_PREFIX struct bn_s* b, BN_VAR_PREFIX struct bn_s* c);
void bignum_signed_sub(BN_VAR_PREFIX struct bn_s* a, BN_VAR_PREFIX struct bn_s* b, BN_VAR_PREFIX struct bn_s* c);
void bignum_signed_mul(BN_VAR_PREFIX struct bn_s* a, BN_VAR_PREFIX struct bn_s* b, BN_VAR_PREFIX struct bn_s* c);
void bignum_signed_div(BN_VAR_PREFIX struct bn_s* a, BN_VAR_PREFIX struct bn_s* b, BN_VAR_PREFIX struct bn_s* c);
void bignum_signed_inc(BN_VAR_PREFIX struct bn_s* n);
void bignum_signed_dec(BN_VAR_PREFIX struct bn_s* n);
int bignum_signed_cmp(BN_VAR_PREFIX struct bn_s* a, BN_VAR_PREFIX struct bn_s* b);
void bignum_signed_assign(BN_VAR_PREFIX struct bn_s* dst, BN_VAR_PREFIX struct bn_s* src);
int bignum_signed_is_zero(BN_VAR_PREFIX struct bn_s* n);
int bignum_signed_is_negative(BN_VAR_PREFIX struct bn_s* n);

#endif
