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

void bigfloat_init(struct bf* n);
void bigfloat_from_bn(struct bf* n, struct bn* bigint);
void bigfloat_from_bn_s(struct bf* n, struct bn_s* bigint);
void to_bignum_signed(struct bn_s* dst, struct bf* n);

//void bigfloat_from_double(struct bf* n, double d);
//double bigfloat_to_double(struct bf* n);

void bigfloat_from_int(struct bf* n, int64_t value);
void bigfloat_add(struct bf* a, struct bf* b, struct bf* c);
void bigfloat_sub(struct bf* a, struct bf* b, struct bf* c);
void bigfloat_mul(struct bf* a, struct bf* b, struct bf* c);
void bigfloat_div(struct bf* a, struct bf* b, struct bf* c);
int bigfloat_cmp(struct bf* a, struct bf* b);
void bigfloat_assign(struct bf* dst, struct bf* src);
int bigfloat_is_zero(struct bf* n);

void bigfloat_normalize(struct bf* n);//should happen automatically, but may be useful if you are manipulating the bf type


#endif
