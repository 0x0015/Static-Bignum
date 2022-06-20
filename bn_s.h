#ifndef __BIGNUM_SIGNED_H__
#define __BIGNUM_SIGNED_H__

#include "bn.h"

typedef struct bn_s{
	uint8_t sign;
	struct bn value;
}bn_s;

#define bn_s_zero ((bn_s){0,bn_zero})

void bignum_signed_init(struct bn_s* n);
void bignum_signed_from_bn(struct bn_s* n, struct bn* bigint);
void bignum_signed_from_int(struct bn_s* n, int64_t value);
int64_t bignum_signed_to_int(struct bn_s* n);

#ifndef BN_NO_STDIO
void bignum_signed_to_string(struct bn_s* n, char* str, int maxsize);
#endif

void bignum_signed_add(struct bn_s* a, struct bn_s* b, struct bn_s* c);
void bignum_signed_sub(struct bn_s* a, struct bn_s* b, struct bn_s* c);
void bignum_signed_mul(struct bn_s* a, struct bn_s* b, struct bn_s* c);
void bignum_signed_div(struct bn_s* a, struct bn_s* b, struct bn_s* c);
void bignum_signed_inc(struct bn_s* n);
void bignum_signed_dec(struct bn_s* n);
int bignum_signed_cmp(struct bn_s* a, struct bn_s* b);
void bignum_signed_assign(struct bn_s* dst, struct bn_s* src);
int bignum_signed_is_zero(struct bn_s* n);
int bignum_signed_is_negative(struct bn_s* n);

#endif
