#ifndef __BIGFLOAT_H__
#define __BIGFLOAT_H__

#include "bn_s.h"

#define BF_NORMALIZATION_MANTISSA_SIZE (BN_BYTES-1)

#define BF_BASE 10

typedef struct bf{
    struct bn_s exponent;
    struct bn_s mantissa;
} bf;

#define bf_zero ((bf){bn_zero, bn_zero})

void bigfloat_init(BN_VAR_PREFIX struct bf* n);
void bigfloat_from_bn(BN_VAR_PREFIX struct bf* n, BN_VAR_PREFIX struct bn* bigint);
void bigfloat_from_bn_s(BN_VAR_PREFIX struct bf* n, BN_VAR_PREFIX struct bn_s* bigint);
void to_bignum_signed(BN_VAR_PREFIX struct bn_s* dst, BN_VAR_PREFIX struct bf* n);

//void bigfloat_from_double(BN_VAR_PREFIX struct bf* n, double d);
//double bigfloat_to_double(BN_VAR_PREFIX struct bf* n);

void bigfloat_from_int(BN_VAR_PREFIX struct bf* n, int64_t value);
void bigfloat_add(BN_VAR_PREFIX struct bf* a, BN_VAR_PREFIX struct bf* b, BN_VAR_PREFIX struct bf* c);
void bigfloat_sub(BN_VAR_PREFIX struct bf* a, BN_VAR_PREFIX struct bf* b, BN_VAR_PREFIX struct bf* c);
void bigfloat_mul(BN_VAR_PREFIX struct bf* a, BN_VAR_PREFIX struct bf* b, BN_VAR_PREFIX struct bf* c);
void bigfloat_div(BN_VAR_PREFIX struct bf* a, BN_VAR_PREFIX struct bf* b, BN_VAR_PREFIX struct bf* c);
void bigfloat_inc(BN_VAR_PREFIX struct bf* n);
void bigfloat_dec(BN_VAR_PREFIX struct bf* n);
int bigfloat_cmp(BN_VAR_PREFIX struct bf* a, BN_VAR_PREFIX struct bf* b);
void bigfloat_assign(BN_VAR_PREFIX struct bf* dst, BN_VAR_PREFIX struct bf* src);
int bigfloat_is_zero(BN_VAR_PREFIX struct bf* n);

void bigfloat_normalize(BN_VAR_PREFIX struct bf* n);//should happen automatically, but may be useful if you are manipulating the bf type


#endif
