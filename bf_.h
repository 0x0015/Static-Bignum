typedef struct PPCAT(bf, BN_PREFIX){
    struct PPCAT(bn_s, BN_PREFIX) exponent;
    struct PPCAT(bn_s, BN_PREFIX) mantissa;
} PPCAT(bf, BN_PREFIX);

//#define PPCAT(bf, BN_PREFIX)_zero ((PPCAT(bf, BN_PREFIX)){bn_zero, bn_zero})

void PPCAT(bigfloat_init, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n);
void PPCAT(bigfloat_from_bn, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* bigint);
void PPCAT(bigfloat_from_bn_s, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* bigint);
void PPCAT(to_bignum_signed, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* dst, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n);

//only define conversions to and from double, if it is known that there is double math implemented
#if (!defined(BN_NO_STDMATH)) || defined(BN_CUSTOM_FMATH) || defined(BN_NATIVE_FMATH)
void PPCAT(bigfloat_from_double, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n, double d);
double PPCAT(bigfloat_to_double, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n);
#endif

void PPCAT(bigfloat_from_int, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n, int64_t value);
void PPCAT(bigfloat_add, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* c);
void PPCAT(bigfloat_sub, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* c);
void PPCAT(bigfloat_mul, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* c);
void PPCAT(bigfloat_div, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* c);
void PPCAT(bigfloat_inc, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n);
void PPCAT(bigfloat_dec, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n);
int PPCAT(bigfloat_cmp, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* b);
void PPCAT(bigfloat_assign, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* dst, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* src);
int PPCAT(bigfloat_is_zero, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n);

void PPCAT(bigfloat_normalize, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n);//should happen automatically, but may be useful if you are manipulating the PPCAT(bf, BN_PREFIX) type

