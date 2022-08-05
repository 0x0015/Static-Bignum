
/* Functions for shifting number in-place. */
static void PPCAT(_lshift_one_bit, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a);
static void PPCAT(_rshift_one_bit, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a);
static void PPCAT(_lshift_word, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, int nwords);
static void PPCAT(_rshift_word, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, int nwords);



/* Public / Exported functions. */
void PPCAT(bignum_init, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n)
{
  require(n, "n is null");

  int i;
  for (i = 0; i < (BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD); ++i)
  {
    n->array[i] = 0;
  }
}


void PPCAT(bignum_from_int, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n, DTYPE_TMP i)
{
  require(n, "n is null");

  PPCAT(bignum_init, BN_PREFIX)(n);

  /* Endianness issue if machine is not little-endian? */
#ifdef WORD_SIZE
 #if (WORD_SIZE == 1)
  n->array[0] = (i & 0x000000ff);
  n->array[1] = (i & 0x0000ff00) >> 8;
  n->array[2] = (i & 0x00ff0000) >> 16;
  n->array[3] = (i & 0xff000000) >> 24;
 #elif (WORD_SIZE == 2)
  n->array[0] = (i & 0x0000ffff);
  n->array[1] = (i & 0xffff0000) >> 16;
 #elif (WORD_SIZE == 4)
  n->array[0] = i;
  DTYPE_TMP num_32 = 32;
  DTYPE_TMP tmp = i >> num_32; /* bit-shift with U64 operands to force 64-bit results */
  n->array[1] = tmp;
 #endif
#endif
}


uint32_t PPCAT(bignum_to_int, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n)
{
  require(n, "n is null");

  uint32_t ret = 0;

  /* Endianness issue if machine is not little-endian? */
#if (WORD_SIZE == 1)
  ret += n->array[0];
  ret += n->array[1] << 8;
  ret += n->array[2] << 16;
  ret += n->array[3] << 24;  
#elif (WORD_SIZE == 2)
  ret += n->array[0];
  ret += n->array[1] << 16;
#elif (WORD_SIZE == 4)
  ret += n->array[0];
#endif

  return ret;
}

DTYPE PPCAT(bignum_to_DTYPE, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n){
	require(n, "n is null");
	return(n->array[0]);
}

#ifndef BN_NO_STDIO
void PPCAT(bignum_from_string, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n, char* str, int nbytes)
{
  require(n, "n is null");
  require(str, "str is null");
  require(nbytes > 0, "nbytes must be positive");
  require((nbytes & 1) == 0, "string format must be in hex -> equal number of bytes");
  require((nbytes % (sizeof(DTYPE) * 2)) == 0, "string length must be a multiple of (sizeof(DTYPE) * 2) characters");
  
  PPCAT(bignum_init, BN_PREFIX)(n);

  DTYPE tmp;                        /* DTYPE is defined in PPCAT(bn, BN_PREFIX).h - uint{8,16,32,64}_t */
  int i = nbytes - (2 * WORD_SIZE); /* index into string */
  int j = 0;                        /* index into array */

  /* reading last hex-byte "MSB" from string first -> big endian */
  /* MSB ~= most significant byte / block ? :) */
  while (i >= 0)
  {
    tmp = 0;
    sscanf(&str[i], SSCANF_FORMAT_STR, &tmp);
    n->array[j] = tmp;
    i -= (2 * WORD_SIZE); /* step WORD_SIZE hex-byte(s) back in the string. */
    j += 1;               /* step one element forward in the array. */
  }
}


void PPCAT(bignum_to_string, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n, char* str, int nbytes)
{
  require(n, "n is null");
  require(str, "str is null");
  require(nbytes > 0, "nbytes must be positive");
  require((nbytes & 1) == 0, "string format must be in hex -> equal number of bytes");

  int j = (BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD) - 1; /* index into array - reading "MSB" first -> big-endian */
  int i = 0;                 /* index into string representation. */

  /* reading last array-element "MSB" first -> big endian */
  while ((j >= 0) && (nbytes > (i + 1)))
  {
    sprintf(&str[i], SPRINTF_FORMAT_STR, n->array[j]);
    i += (2 * WORD_SIZE); /* step WORD_SIZE hex-byte(s) forward in the string. */
    j -= 1;               /* step one element back in the array. */
  }

  /* Count leading zeros: */
  j = 0;
  while (str[j] == '0')
  {
    j += 1;
  }
 
  /* Move string j places ahead, effectively skipping leading zeros */ 
  for (i = 0; i < (nbytes - j); ++i)
  {
    str[i] = str[i + j];
  }

  /* Zero-terminate string */
  str[i] = 0;
}
#endif


void PPCAT(bignum_dec, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n)
{
  require(n, "n is null");

  DTYPE tmp; /* copy of n */
  DTYPE res;

  int i;
  for (i = 0; i < (BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD); ++i)
  {
    tmp = n->array[i];
    res = tmp - 1;
    n->array[i] = res;

    if (!(res > tmp))
    {
      break;
    }
  }
}


void PPCAT(bignum_inc, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n)
{
  require(n, "n is null");

  DTYPE res;
  DTYPE_TMP tmp; /* copy of n */

  int i;
  for (i = 0; i < (BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD); ++i)
  {
    tmp = n->array[i];
    res = tmp + 1;
    n->array[i] = res;

    if (res > tmp)
    {
      break;
    }
  }
}


void PPCAT(bignum_add, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* c)
{
  require(a, "a is null");
  require(b, "b is null");
  require(c, "c is null");

  DTYPE_TMP tmp;
  int carry = 0;
  int i;
  for (i = 0; i < (BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD); ++i)
  {
    tmp = (DTYPE_TMP)a->array[i] + b->array[i] + carry;
    carry = (tmp > MAX_VAL);
    c->array[i] = (tmp & MAX_VAL);
  }
}


void PPCAT(bignum_sub, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* c)
{
  require(a, "a is null");
  require(b, "b is null");
  require(c, "c is null");

  DTYPE_TMP res;
  DTYPE_TMP tmp1;
  DTYPE_TMP tmp2;
  int borrow = 0;
  int i;
  for (i = 0; i < (BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD); ++i)
  {
    tmp1 = (DTYPE_TMP)a->array[i] + (MAX_VAL + 1); /* + number_base */
    tmp2 = (DTYPE_TMP)b->array[i] + borrow;;
    res = (tmp1 - tmp2);
    c->array[i] = (DTYPE)(res & MAX_VAL); /* "modulo number_base" == "% (number_base - 1)" if number_base is 2^N */
    borrow = (res <= MAX_VAL);
  }
}


void PPCAT(bignum_mul, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* c)
{
  require(a, "a is null");
  require(b, "b is null");
  require(c, "c is null");

  struct PPCAT(bn, BN_PREFIX) row;
  struct PPCAT(bn, BN_PREFIX) tmp;
  int i, j;

  PPCAT(bignum_init, BN_PREFIX)(c);

  for (i = 0; i < (BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD); ++i)
  {
    PPCAT(bignum_init, BN_PREFIX)(&row);

    for (j = 0; j < (BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD); ++j)
    {
      if (i + j < (BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD))
      {
        PPCAT(bignum_init, BN_PREFIX)(&tmp);
        DTYPE_TMP intermediate = ((DTYPE_TMP)a->array[i] * (DTYPE_TMP)b->array[j]);
        PPCAT(bignum_from_int, BN_PREFIX)(&tmp, intermediate);
        PPCAT(_lshift_word, BN_PREFIX)(&tmp, i + j);
        PPCAT(bignum_add, BN_PREFIX)(&tmp, &row, &row);
      }
    }
    PPCAT(bignum_add, BN_PREFIX)(c, &row, c);
  }
}


void PPCAT(bignum_div, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* c)
{
  require(a, "a is null");
  require(b, "b is null");
  require(c, "c is null");

  if(PPCAT(bignum_is_zero, BN_PREFIX)(b)){
	  PPCAT(bignum_init, BN_PREFIX)(c);
	  return;
  }

  struct PPCAT(bn, BN_PREFIX) current;
  struct PPCAT(bn, BN_PREFIX) denom;
  struct PPCAT(bn, BN_PREFIX) tmp;

  PPCAT(bignum_from_int, BN_PREFIX)(&current, 1);               // int current = 1;
  PPCAT(bignum_assign, BN_PREFIX)(&denom, b);                   // denom = b
  PPCAT(bignum_assign, BN_PREFIX)(&tmp, a);                     // tmp   = a

  const DTYPE_TMP half_max = 1 + (DTYPE_TMP)(MAX_VAL / 2);
  bool overflow = false;
  while (PPCAT(bignum_cmp, BN_PREFIX)(&denom, a) != LARGER)     // while (denom <= a) {
  {
    if (denom.array[(BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD) - 1] >= half_max)
    {
      overflow = true;
      break;
    }
    PPCAT(_lshift_one_bit, BN_PREFIX)(&current);                //   current <<= 1;
    PPCAT(_lshift_one_bit, BN_PREFIX)(&denom);                  //   denom <<= 1;
  }
  if (!overflow)
  {
    PPCAT(_rshift_one_bit, BN_PREFIX)(&denom);                  // denom >>= 1;
    PPCAT(_rshift_one_bit, BN_PREFIX)(&current);                // current >>= 1;
  }
  PPCAT(bignum_init, BN_PREFIX)(c);                             // int answer = 0;

  while (!PPCAT(bignum_is_zero, BN_PREFIX)(&current))           // while (current != 0)
  {
    if (PPCAT(bignum_cmp, BN_PREFIX)(&tmp, &denom) != SMALLER)  //   if (dividend >= denom)
    {
      PPCAT(bignum_sub, BN_PREFIX)(&tmp, &denom, &tmp);         //     dividend -= denom;
      PPCAT(bignum_or, BN_PREFIX)(c, &current, c);              //     answer |= current;
    }
    PPCAT(_rshift_one_bit, BN_PREFIX)(&current);                //   current >>= 1;
    PPCAT(_rshift_one_bit, BN_PREFIX)(&denom);                  //   denom >>= 1;
  }                                           // return answer;
}


void PPCAT(bignum_lshift, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, int nbits)
{
  require(a, "a is null");
  require(b, "b is null");
  require(nbits >= 0, "no negative shifts");

  PPCAT(bignum_assign, BN_PREFIX)(b, a);
  /* Handle shift in multiples of word-size */
  const int nbits_pr_word = (WORD_SIZE * 8);
  int nwords = nbits / nbits_pr_word;
  if (nwords != 0)
  {
    PPCAT(_lshift_word, BN_PREFIX)(b, nwords);
    nbits -= (nwords * nbits_pr_word);
  }

  if (nbits != 0)
  {
    int i;
    for (i = ((BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD) - 1); i > 0; --i)
    {
      b->array[i] = (b->array[i] << nbits) | (b->array[i - 1] >> ((8 * WORD_SIZE) - nbits));
    }
    b->array[i] <<= nbits;
  }
}


void PPCAT(bignum_rshift, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, int nbits)
{
  require(a, "a is null");
  require(b, "b is null");
  require(nbits >= 0, "no negative shifts");
  
  PPCAT(bignum_assign, BN_PREFIX)(b, a);
  /* Handle shift in multiples of word-size */
  const int nbits_pr_word = (WORD_SIZE * 8);
  int nwords = nbits / nbits_pr_word;
  if (nwords != 0)
  {
    PPCAT(_rshift_word, BN_PREFIX)(b, nwords);
    nbits -= (nwords * nbits_pr_word);
  }

  if (nbits != 0)
  {
    int i;
    for (i = 0; i < ((BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD) - 1); ++i)
    {
      b->array[i] = (b->array[i] >> nbits) | (b->array[i + 1] << ((8 * WORD_SIZE) - nbits));
    }
    b->array[i] >>= nbits;
  }
  
}


void PPCAT(bignum_mod, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* c)
{
  /*
    Take divmod and throw away div part
  */
  require(a, "a is null");
  require(b, "b is null");
  require(c, "c is null");

  struct PPCAT(bn, BN_PREFIX) tmp;

  PPCAT(bignum_divmod, BN_PREFIX)(a,b,&tmp,c);
}

void PPCAT(bignum_divmod, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* c, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* d)
{
  /*
    Puts a%b in d
    and a/b in c

    mod(a,b) = a - ((a / b) * b)

    example:
      mod(8, 3) = 8 - ((8 / 3) * 3) = 2
  */
  require(a, "a is null");
  require(b, "b is null");
  require(c, "c is null");

  struct PPCAT(bn, BN_PREFIX) tmp;

  /* c = (a / b) */
  PPCAT(bignum_div, BN_PREFIX)(a, b, c);

  /* tmp = (c * b) */
  PPCAT(bignum_mul, BN_PREFIX)(c, b, &tmp);

  /* c = a - tmp */
  PPCAT(bignum_sub, BN_PREFIX)(a, &tmp, d);
}


void PPCAT(bignum_and, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* c)
{
  require(a, "a is null");
  require(b, "b is null");
  require(c, "c is null");

  int i;
  for (i = 0; i < (BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD); ++i)
  {
    c->array[i] = (a->array[i] & b->array[i]);
  }
}


void PPCAT(bignum_or, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* c)
{
  require(a, "a is null");
  require(b, "b is null");
  require(c, "c is null");

  int i;
  for (i = 0; i < (BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD); ++i)
  {
    c->array[i] = (a->array[i] | b->array[i]);
  }
}


void PPCAT(bignum_xor, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* c)
{
  require(a, "a is null");
  require(b, "b is null");
  require(c, "c is null");

  int i;
  for (i = 0; i < (BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD); ++i)
  {
    c->array[i] = (a->array[i] ^ b->array[i]);
  }
}


int PPCAT(bignum_cmp, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b)
{
  require(a, "a is null");
  require(b, "b is null");

  int i = (BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD);
  do
  {
    i -= 1; /* Decrement first, to start with last array element */
    if (a->array[i] > b->array[i])
    {
      return LARGER;
    }
    else if (a->array[i] < b->array[i])
    {
      return SMALLER;
    }
  }
  while (i != 0);

  return EQUAL;
}


int PPCAT(bignum_is_zero, BN_PREFIX)(BN_VAR_PREFIX const struct PPCAT(bn, BN_PREFIX)* n)
{
  require(n, "n is null");

  int i;
  for (i = 0; i < (BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD); ++i)
  {
    if (n->array[i])
    {
      return 0;
    }
  }

  return 1;
}


void PPCAT(bignum_pow, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* c)
{
  require(a, "a is null");
  require(b, "b is null");
  require(c, "c is null");

  struct PPCAT(bn, BN_PREFIX) tmp;

  PPCAT(bignum_init, BN_PREFIX)(c);

  if (PPCAT(bignum_cmp, BN_PREFIX)(b, c) == EQUAL)
  {
    /* Return 1 when exponent is 0 -- n^0 = 1 */
    PPCAT(bignum_inc, BN_PREFIX)(c);
  }
  else
  {
    struct PPCAT(bn, BN_PREFIX) bcopy;
    PPCAT(bignum_assign, BN_PREFIX)(&bcopy, b);

    /* Copy a -> tmp */
    PPCAT(bignum_assign, BN_PREFIX)(&tmp, a);

    PPCAT(bignum_dec, BN_PREFIX)(&bcopy);
 
    /* Begin summing products: */
    while (!PPCAT(bignum_is_zero, BN_PREFIX)(&bcopy))
    {

      /* c = tmp * tmp */
      PPCAT(bignum_mul, BN_PREFIX)(&tmp, a, c);
      /* Decrement b by one */
      PPCAT(bignum_dec, BN_PREFIX)(&bcopy);

      PPCAT(bignum_assign, BN_PREFIX)(&tmp, c);
    }

    /* c = tmp */
    PPCAT(bignum_assign, BN_PREFIX)(c, &tmp);
  }
}

void PPCAT(bignum_isqrt, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX) *a, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* b)
{
  require(a, "a is null");
  require(b, "b is null");

  struct PPCAT(bn, BN_PREFIX) low, high, mid, tmp;

  PPCAT(bignum_init, BN_PREFIX)(&low);
  PPCAT(bignum_assign, BN_PREFIX)(&high, a);
  PPCAT(bignum_rshift, BN_PREFIX)(&high, &mid, 1);
  PPCAT(bignum_inc, BN_PREFIX)(&mid);

  while (PPCAT(bignum_cmp, BN_PREFIX)(&high, &low) > 0) 
  {
    PPCAT(bignum_mul, BN_PREFIX)(&mid, &mid, &tmp);
    if (PPCAT(bignum_cmp, BN_PREFIX)(&tmp, a) > 0) 
    {
      PPCAT(bignum_assign, BN_PREFIX)(&high, &mid);
      PPCAT(bignum_dec, BN_PREFIX)(&high);
    }
    else 
    {
      PPCAT(bignum_assign, BN_PREFIX)(&low, &mid);
    }
    PPCAT(bignum_sub, BN_PREFIX)(&high,&low,&mid);
    PPCAT(_rshift_one_bit, BN_PREFIX)(&mid);
    PPCAT(bignum_add, BN_PREFIX)(&low,&mid,&mid);
    PPCAT(bignum_inc, BN_PREFIX)(&mid);
  }
  PPCAT(bignum_assign, BN_PREFIX)(b,&low);
}


void PPCAT(bignum_assign, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* dst, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* src)
{
  require(dst, "dst is null");
  require(src, "src is null");

  int i;
  for (i = 0; i < (BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD); ++i)
  {
    dst->array[i] = src->array[i];
  }
}


/* Private / Static functions. */
static void PPCAT(_rshift_word, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, int nwords)
{
  /* Naive method: */
  require(a, "a is null");
  require(nwords >= 0, "no negative shifts");

  int i;
  if (nwords >= (BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD))
  {
    for (i = 0; i < (BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD); ++i)
    {
      a->array[i] = 0;
    }
    return;
  }

  for (i = 0; i < (BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD) - nwords; ++i)
  {
    a->array[i] = a->array[i + nwords];
  }
  for (; i < (BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD); ++i)
  {
    a->array[i] = 0;
  }
}


static void PPCAT(_lshift_word, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a, int nwords)
{
  require(a, "a is null");
  require(nwords >= 0, "no negative shifts");

  int i;
  /* Shift whole words */
  for (i = ((BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD) - 1); i >= nwords; --i)
  {
    a->array[i] = a->array[i - nwords];
  }
  /* Zero pad shifted words. */
  for (; i >= 0; --i)
  {
    a->array[i] = 0;
  }  
}


static void PPCAT(_lshift_one_bit, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a)
{
  require(a, "a is null");

  int i;
  for (i = ((BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD) - 1); i > 0; --i)
  {
    a->array[i] = (a->array[i] << 1) | (a->array[i - 1] >> ((8 * WORD_SIZE) - 1));
  }
  a->array[0] <<= 1;
}


static void PPCAT(_rshift_one_bit, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* a)
{
  require(a, "a is null");

  int i;
  for (i = 0; i < ((BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD) - 1); ++i)
  {
    a->array[i] = (a->array[i] >> 1) | (a->array[i + 1] << ((8 * WORD_SIZE) - 1));
  }
  a->array[(BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD) - 1] >>= 1;
}

//I don't know how to test this in any other way
#if defined(__GNUC__) || defined(__builtin_clz)
	#define BN_GNU_CLZ
#endif

#ifndef BN_CUSTOM_BSR
	#ifdef BN_GNU_CLZ
		#define BN_CLZ __builtin_clz
	#else
		#ifdef BN_CLZ_CLZ
			#define BN_CLZ clz
		#else
			//no bsr is defined
			#define BN_NO_CLZ
		#endif
	#endif
#else
	#define BN_CLZ BN_CUSTOM_BSR
#endif

#ifdef BN_NO_CLZ
//based on the anwer from https://stackoverflow.com/questions/8991024/find-out-how-many-binary-digits-a-particular-integer-has
unsigned int PPCAT(bignum_bsr, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n){
	require(n, "n is null");

	unsigned int digits = 0;
	struct PPCAT(bn, BN_PREFIX) tmp;
	struct PPCAT(bn, BN_PREFIX) zero;
	PPCAT(bignum_assign, BN_PREFIX)(&tmp, n);
	PPCAT(bignum_init, BN_PREFIX)(&zero);
	while(1){
		int result = PPCAT(bignum_cmp, BN_PREFIX)(&tmp, &zero);
		if(result != LARGER){
			break;
		}
		digits++;
		struct PPCAT(bn, BN_PREFIX) shft_tmp;
		PPCAT(bignum_assign, BN_PREFIX)(&shft_tmp, &tmp);
		PPCAT(bignum_rshift, BN_PREFIX)(&shft_tmp, &tmp, 1);
	}
	return(digits);
}

#else

unsigned int PPCAT(bignum_bsr, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n){
	unsigned int output = 0;
	for(unsigned int i=(BN_ARRAY_SIZE * BN_ARRAY_SIZE_MOD)-1;i>0;i--){
		if(n->array[i] == 0){
			output += WORD_SIZE * 8;
		}else{
			unsigned int secLZ = BN_CLZ(n->array[i]);
			output += secLZ;
			return(output);
		}
	}
	return(output);
}

#endif
