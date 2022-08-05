
typedef struct PPCAT(bn_s, BN_PREFIX){
	uint8_t sign;
	struct PPCAT(bn, BN_PREFIX) value;
}PPCAT(bn_s, BN_PREFIX);

/*#define PPCAT(bn_s, BN_PREFIX)_zero ((PPCAT(bn_s, BN_PREFIX)){0,bn_zero})*/

void PPCAT(bignum_signed_init, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* n);
void PPCAT(bignum_signed_from_bn, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* n, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* bigint);
void PPCAT(bignum_signed_from_int, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* n, int64_t value);
int32_t PPCAT(bignum_signed_to_int, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* n);

#ifndef BN_NO_STDIO
void PPCAT(bignum_signed_to_string, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* n, char* str, int maxsize);
#endif

void PPCAT(bignum_signed_add, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* c);
void PPCAT(bignum_signed_sub, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* c);
void PPCAT(bignum_signed_mul, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* c);
void PPCAT(bignum_signed_div, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* c);
void PPCAT(bignum_signed_inc, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* n);
void PPCAT(bignum_signed_dec, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* n);
int PPCAT(bignum_signed_cmp, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* b);
void PPCAT(bignum_signed_assign, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* dst, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* src);
int PPCAT(bignum_signed_is_zero, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* n);
int PPCAT(bignum_signed_is_negative, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* n);
