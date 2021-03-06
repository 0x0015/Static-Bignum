#ifndef __BIGNUM_H__
#define __BIGNUM_H__
/*

Big number library - arithmetic on multiple-precision unsigned integers.

This library is an implementation of arithmetic on arbitrarily large integers.

The difference between this and other implementations, is that the data structure
has optimal memory utilization (i.e. a 1024 bit integer takes up 128 bytes RAM),
and all memory is allocated statically: no dynamic allocation for better or worse.

Primary goals are correctness, clarity of code and clean, portable implementation.
Secondary goal is a memory footprint small enough to make it suitable for use in
embedded applications.


The current state is correct functionality and adequate performance.
There may well be room for performance-optimizations and improvements.

*/
//this is an opencl support thing, so I can set this to private or whatever
#ifndef BN_VAR_PREFIX
#define BN_VAR_PREFIX 
#endif

/*Uncomment line below if stdint shouldn't be included (eg. opencl)*/
//#define BN_NO_STDINT

/*Uncomment line below if stdio shouldn't be included (disables to/from string operations)*/
//#define BN_NO_STDIO

//#define BN_NO_ASSERT

//#define BN_NO_STDBOOL

#ifndef BN_NO_STDINT
#include <stdint.h>
#else
typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
#endif

#ifndef BN_NO_ASSERT
#include <assert.h>
#else
#define assert(a) 
#endif

#define BN_BYTES 32 //must be divisable by the word size
#define BN_BITS (BN_BYTES*8)
/* This macro defines the word size in bytes of the array that constitues the big-number data structure. */
#ifndef WORD_SIZE
  #define WORD_SIZE 4
#endif

/* Size of big-numbers in bytes */
#define BN_ARRAY_SIZE    (BN_BYTES / WORD_SIZE)


/* Here comes the compile-time specialization for how large the underlying array size should be. */
/* The choices are 1, 2 and 4 bytes in size with uint32, uint64 for WORD_SIZE==4, as temporary. */
#ifndef WORD_SIZE
  #error Must define WORD_SIZE to be 1, 2, 4
#elif (WORD_SIZE == 1)
  /* Data type of array in structure */
  #define DTYPE                    uint8_t
  /* bitmask for getting MSB */
  #define DTYPE_MSB                ((DTYPE_TMP)(0x80))
  /* Data-type larger than DTYPE, for holding intermediate results of calculations */
  #define DTYPE_TMP                uint32_t
  /* sprintf format string */
  #define SPRINTF_FORMAT_STR       "%.02x"
  #define SSCANF_FORMAT_STR        "%2hhx"
  /* Max value of integer type */
  #define MAX_VAL                  ((DTYPE_TMP)0xFF)
#elif (WORD_SIZE == 2)
  #define DTYPE                    uint16_t
  #define DTYPE_TMP                uint32_t
  #define DTYPE_MSB                ((DTYPE_TMP)(0x8000))
  #define SPRINTF_FORMAT_STR       "%.04x"
  #define SSCANF_FORMAT_STR        "%4hx"
  #define MAX_VAL                  ((DTYPE_TMP)0xFFFF)
#elif (WORD_SIZE == 4)
  #define DTYPE                    uint32_t
  #define DTYPE_TMP                uint64_t
  #define DTYPE_MSB                ((DTYPE_TMP)(0x80000000))
  #define SPRINTF_FORMAT_STR       "%.08x"
  #define SSCANF_FORMAT_STR        "%8x"
  #define MAX_VAL                  ((DTYPE_TMP)0xFFFFFFFF)
#endif
#ifndef DTYPE
  #error DTYPE must be defined to uint8_t, uint16_t uint32_t or whatever
#endif


/* Custom assert macro - easy to disable */
#define require(p, msg) assert(p && msg)


/* Data-holding structure: array of DTYPEs */
typedef struct bn
{
  DTYPE array[BN_ARRAY_SIZE];
} bn;

#define bn_zero ((bn){0})
//#define bn_max ((bn){{[0 ... BN_ARRAY_SIZE-1] = MAX_VAL}})

/* Tokens returned by bignum_cmp() for value comparison */
enum { SMALLER = -1, EQUAL = 0, LARGER = 1 };

/* Initialization functions: */
void bignum_init(BN_VAR_PREFIX struct bn* n);
void bignum_from_int(BN_VAR_PREFIX struct bn* n, DTYPE_TMP i);
uint32_t  bignum_to_int(BN_VAR_PREFIX struct bn* n);
DTYPE  bignum_to_DTYPE(BN_VAR_PREFIX struct bn* n);

#ifndef BN_NO_STDIO
void bignum_from_string(BN_VAR_PREFIX struct bn* n, char* str, int nbytes);
void bignum_to_string(BN_VAR_PREFIX struct bn* n, char* str, int maxsize);
#endif

/* Basic arithmetic operations: */
void bignum_add(BN_VAR_PREFIX struct bn* a, BN_VAR_PREFIX struct bn* b, BN_VAR_PREFIX struct bn* c); /* c = a + b */
void bignum_sub(BN_VAR_PREFIX struct bn* a, BN_VAR_PREFIX struct bn* b, BN_VAR_PREFIX struct bn* c); /* c = a - b */
void bignum_mul(BN_VAR_PREFIX struct bn* a, BN_VAR_PREFIX struct bn* b, BN_VAR_PREFIX struct bn* c); /* c = a * b */
void bignum_div(BN_VAR_PREFIX struct bn* a, BN_VAR_PREFIX struct bn* b, BN_VAR_PREFIX struct bn* c); /* c = a / b */
void bignum_mod(BN_VAR_PREFIX struct bn* a, BN_VAR_PREFIX struct bn* b, BN_VAR_PREFIX struct bn* c); /* c = a % b */
void bignum_divmod(BN_VAR_PREFIX struct bn* a, BN_VAR_PREFIX struct bn* b, BN_VAR_PREFIX struct bn* c, BN_VAR_PREFIX struct bn* d); /* c = a/b, d = a%b */

/* Bitwise operations: */
void bignum_and(BN_VAR_PREFIX struct bn* a, BN_VAR_PREFIX struct bn* b, BN_VAR_PREFIX struct bn* c); /* c = a & b */
void bignum_or(BN_VAR_PREFIX struct bn* a, BN_VAR_PREFIX struct bn* b, BN_VAR_PREFIX struct bn* c);  /* c = a | b */
void bignum_xor(BN_VAR_PREFIX struct bn* a, BN_VAR_PREFIX struct bn* b, BN_VAR_PREFIX struct bn* c); /* c = a ^ b */
void bignum_lshift(BN_VAR_PREFIX struct bn* a, BN_VAR_PREFIX struct bn* b, int nbits); /* b = a << nbits */
void bignum_rshift(BN_VAR_PREFIX struct bn* a, BN_VAR_PREFIX struct bn* b, int nbits); /* b = a >> nbits */

unsigned int bignum_bsr(BN_VAR_PREFIX struct bn* n);

/* Special operators and comparison */
int  bignum_cmp(BN_VAR_PREFIX struct bn* a, BN_VAR_PREFIX struct bn* b);               /* Compare: returns LARGER, EQUAL or SMALLER */
int  bignum_is_zero(BN_VAR_PREFIX const struct bn* n);                         /* For comparison with zero */
void bignum_inc(BN_VAR_PREFIX struct bn* n);                             /* Increment: add one to n */
void bignum_dec(BN_VAR_PREFIX struct bn* n);                             /* Decrement: subtract one from n */
void bignum_pow(BN_VAR_PREFIX struct bn* a, BN_VAR_PREFIX struct bn* b, BN_VAR_PREFIX struct bn* c); /* Calculate a^b -- e.g. 2^10 => 1024 */
void bignum_isqrt(BN_VAR_PREFIX struct bn* a, BN_VAR_PREFIX struct bn* b);             /* Integer square root -- e.g. isqrt(5) => 2*/
void bignum_assign(BN_VAR_PREFIX struct bn* dst, BN_VAR_PREFIX struct bn* src);        /* Copy src into dst -- dst := src */


#endif /* #ifndef __BIGNUM_H__ */


