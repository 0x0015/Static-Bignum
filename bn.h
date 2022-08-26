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
/*this is an opencl support thing, so I can set this to private or whatever*/
#ifndef BN_VAR_PREFIX
#define BN_VAR_PREFIX 
#endif

/*Uncomment line below if stdint shouldn't be included (eg. opencl)*/
/*#define BN_NO_STDINT*/

/*Uncomment line below if stdio shouldn't be included (disables to/from string operations)*/
/*#define BN_NO_STDIO  */

/*#define BN_NO_ASSERT */

/*#define BN_NO_STDBOOL*/

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

#define BN_BYTES 32 /*must be divisable by the word size*/
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

/* Tokens returned by bignum_cmp() for value comparison */
enum { SMALLER = -1, EQUAL = 0, LARGER = 1 };

/*#define PPCAT(a, ...) PRIMITIVE_CAT(a, __VA_ARGS__)*/
/*#define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__*/
/*veradic is really not needed, so let's not use it :)*/
#define PPCAT(a, b) PRIMITIVE_CAT(a, b)
#define PRIMITIVE_CAT(a, b) a ## b

#define BN_PREFIX
#define BN_ARRAY_SIZE_MOD 1
#include "bn_.h"
#undef BN_PREFIX
#undef BN_ARRAY_SIZE_MOD

#ifndef BN_NO_DOUBLE_P
#define BN_PREFIX _2
#define BN_ARRAY_SIZE_MOD 2
#include "bn_.h"
#undef BN_PREFIX
#undef BN_ARRAY_SIZE_MOD

void bignum_to_2(BN_VAR_PREFIX struct bn_2* dst, BN_VAR_PREFIX struct bn* src);
void bignum_2_to_1(BN_VAR_PREFIX struct bn* dst, BN_VAR_PREFIX struct bn_2* src);
#endif

#endif /* #ifndef __BIGNUM_H__ */
