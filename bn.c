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

#ifndef BN_NO_STDIO
#include <stdio.h>
#endif
#ifndef BN_NO_STDBOOL
#include <stdbool.h>
#else
#define bool uint8_t
#define true 1
#define false 0
#endif
#ifndef BN_NO_ASSERT
#include <assert.h>
#endif
#include "bn.h"

#define BN_PREFIX
#define BN_ARRAY_SIZE_MOD 1
#include "bn_.c"
#undef BN_PREFIX
#undef BN_ARRAY_SIZE_MOD
#define BN_PREFIX _2
#define BN_ARRAY_SIZE_MOD 2
#include "bn_.c"
#undef BN_PREFIX
#undef BN_ARRAY_SIZE_MOD

/*add a conversion between bn and bn_2 here*/
