
typedef struct PPCAT(bn, BN_PREFIX)
{
  DTYPE array[BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD];
} PPCAT(bn, BN_PREFIX);

/*#define PPCAT(bn, BN_PREFIX)_zero ((PPCAT(bn, BN_PREFIX)){0})*/
/*#define PPCAT(bn, BN_PREFIX)_max ((PPCAT(bn, BN_PREFIX)){{[0 ... BN_ARRAY_SIZE-1] = MAX_VAL}})*/


/* Initialization functions: */
void PPCAT(bignum_init, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n);
void PPCAT(bignum_from_int, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n, DTYPE_TMP i);
uint32_t  PPCAT(bignum_to_int, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n);
DTYPE  PPCAT(bignum_to_DTYPE, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n);

#ifndef BN_NO_STDIO
void PPCAT(bignum_from_string, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n, char* str, int nbytes);
void PPCAT(bignum_to_string, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n, char* str, int maxsize);
#endif

/* Basic arithmetic operations: */
void PPCAT(bignum_add, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* c); /* c = a + b */
void PPCAT(bignum_sub, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* c); /* c = a - b */
void PPCAT(bignum_mul, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* c); /* c = a * b */
void PPCAT(bignum_div, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* c); /* c = a / b */
void PPCAT(bignum_mod, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* c); /* c = a % b */
void PPCAT(bignum_divmod, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* c, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* d); /* c = a/b, d = a%b */

/* Bitwise operations: */
void PPCAT(bignum_and, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* c); /* c = a & b */
void PPCAT(bignum_or, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* c);  /* c = a | b */
void PPCAT(bignum_xor, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* c); /* c = a ^ b */
void PPCAT(bignum_lshift, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, int nbits); /* b = a << nbits */
void PPCAT(bignum_rshift, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, int nbits); /* b = a >> nbits */

unsigned int PPCAT(bignum_bsr, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n);

/* Special operators and comparison */
int  PPCAT(bignum_cmp, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b);               /* Compare: returns LARGER, EQUAL or SMALLER */
int  PPCAT(bignum_is_zero, BN_PREFIX)(BN_VAR_PREFIX const struct PPCAT(bn, BN_PREFIX)* n);                         /* For comparison with zero */
void PPCAT(bignum_inc, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n);                             /* Increment: add one to n */
void PPCAT(bignum_dec, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n);                             /* Decrement: subtract one from n */
void PPCAT(bignum_pow, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* c); /* Calculate a^b -- e.g. 2^10 => 1024 */
void PPCAT(bignum_isqrt, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b);             /* Integer square root -- e.g. isqrt(5) => 2*/
void PPCAT(bignum_assign, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* dst, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* src);        /* Copy src into dst -- dst := src */

