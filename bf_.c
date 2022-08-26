
#ifdef BF_DIAGNOSTICS
#include <stdio.h>

void PPCAT(bf_diagnostic_print, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n){
	double d = PPCAT(bigfloat_to_double, BN_PREFIX)(n);
	char buf1[1000];
	char buf2[1000];
	int64_t man = PPCAT(bignum_signed_to_int, BN_PREFIX)(&n->mantissa);
	int64_t exp = PPCAT(bignum_signed_to_int, BN_PREFIX)(&n->exponent);
	PPCAT(bignum_signed_to_string, BN_PREFIX)(&n->mantissa, buf1, 1000);
	PPCAT(bignum_signed_to_string, BN_PREFIX)(&n->exponent, buf2, 1000);
	printf("mantissa: %s (%li) exponent: %s (%li)   (%lf)\n", buf1, man, buf2, exp, d);
}
#define BF_IF_DIAGNOSTIC_INLINE(x) x
#else
#define BF_IF_DIAGNOSTIC_INLINE(x) 
#endif

#if BN_ARRAY_SIZE_MOD == 1
void PPCAT(bigfloat_change_exponent, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n, int wantedDigits);
#else
void bigfloat_change_exponent_2(struct bf_2* n, int wantedDigits);
#endif
int PPCAT(bf_get_maxDigits, BN_PREFIX)();
void PPCAT(bf_shiftEXP, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n, int shift);
unsigned int PPCAT(numPlaces, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n);

void PPCAT(bigfloat_init, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n){
	PPCAT(bignum_signed_init, BN_PREFIX)(&n->exponent);
	PPCAT(bignum_signed_init, BN_PREFIX)(&n->mantissa);
}

void PPCAT(bigfloat_from_bn, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* bigint){
	PPCAT(bignum_signed_from_bn, BN_PREFIX)(&n->mantissa, bigint);
	PPCAT(bignum_signed_from_int, BN_PREFIX)(&n->exponent, 0);
	PPCAT(bigfloat_normalize, BN_PREFIX)(n);
}

void PPCAT(bigfloat_from_bn_s, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* bigint_signed){
	PPCAT(bignum_signed_assign, BN_PREFIX)(&n->mantissa, bigint_signed);
	PPCAT(bignum_signed_from_int, BN_PREFIX)(&n->exponent, 0);
	PPCAT(bigfloat_normalize, BN_PREFIX)(n);
}

void PPCAT(bigfloat_from_int, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n, int64_t value){
	PPCAT(bignum_signed_from_int, BN_PREFIX)(&n->mantissa, value);
	PPCAT(bignum_signed_from_int, BN_PREFIX)(&n->exponent, 0);
	PPCAT(bigfloat_normalize, BN_PREFIX)(n);
}

#if (!defined(BN_NO_STDMATH)) || defined(BN_CUSTOM_FMATH) || defined(BN_NATIVE_FMATH)
void PPCAT(bigfloat_from_double, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n, double val){
	double d = val;
	if(val < 0){
		d = -d;
	}
	double rd = log(d)/log((double)BF_BASE);//how many powers of PPCAT(bf, BN_PREFIX)_base fit in d?
	int r = rd;
	int er = 0;
	if(r == 0){
		if(d < 1 && d > 0){
			r--;
		}
	}else if(r < 0){
		r--;
	}
	if(bf_closeWithinEpsilon(r, rd)){
		BF_IF_DIAGNOSTIC_INLINE(printf("multiple of BF_BASE\n");)
		er--;
	}
	BF_IF_DIAGNOSTIC_INLINE(printf("r=%i (rd=%lf)\n", r, rd);)
	double rDIV = d / pow((double)BF_BASE, (double)r);
	unsigned int rmax = log((double)MAX_VAL)/log((double)BF_BASE);
	int64_t mres = rDIV * pow((double)BF_BASE, (double)rmax);
	unsigned int mresr = log((double)mres)/log((double)BF_BASE);
	int64_t exp = -mresr;
	PPCAT(bignum_signed_from_int, BN_PREFIX)(&n->mantissa, mres);
	PPCAT(bignum_signed_from_int, BN_PREFIX)(&n->exponent, exp+r+er);
	BF_IF_DIAGNOSTIC_INLINE(printf("man: %li, exp: %li\n", mres, exp+r+er);)
	if(val < 0){
		n->mantissa.sign = 1;
	}
	PPCAT(bigfloat_normalize, BN_PREFIX)(n);
}
double PPCAT(bigfloat_to_double, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n){
	struct PPCAT(bf, BN_PREFIX) tmp;
	PPCAT(bigfloat_assign, BN_PREFIX)(&tmp, n); 
	unsigned int rmax = log((double)MAX_VAL)/log((double)BF_BASE);
	PPCAT(bigfloat_change_exponent, BN_PREFIX)(&tmp, rmax);
	int64_t man = PPCAT(bignum_signed_to_int, BN_PREFIX)(&tmp.mantissa);
	int64_t exp = PPCAT(bignum_signed_to_int, BN_PREFIX)(&tmp.exponent);
	//unsigned int r = log((double)man)/log((double)BF_BASE);  //unused
	BF_IF_DIAGNOSTIC_INLINE(printf("man: %li, exp: %li\n", man, exp);)
	double output = man;
	output *= pow((double)BF_BASE, (double)exp);
	return(output);
}
#endif

//based off of https://en.wikipedia.org/wiki/Floating-point_arithmetic#Floating-point_operations

void PPCAT(bigfloat_add, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* c){
	int expdif = PPCAT(bignum_signed_cmp, BN_PREFIX)(&a->exponent, &b->exponent);
	switch(expdif){
		case EQUAL:{
				   BF_IF_DIAGNOSTIC_INLINE(printf("add, equal\n");)
				   //just add the mantissa
#ifdef BF_DIAGNOSTICS
				     printf("a:  ");
				     PPCAT(bf_diagnostic_print, BN_PREFIX)(a);
				     printf("b:  ");
				     PPCAT(bf_diagnostic_print, BN_PREFIX)(b);
#endif
				   //PPCAT(bf_shiftEXP, BN_PREFIX)(a, 1);
				   //PPCAT(bf_shiftEXP, BN_PREFIX)(b, 1);
				   PPCAT(bignum_signed_add, BN_PREFIX)(&a->mantissa, &b->mantissa, &c->mantissa);
				   PPCAT(bignum_signed_assign, BN_PREFIX)(&c->exponent, &a->exponent);
#ifdef BF_DIAGNOSTICS
				     printf("pre normalization:  ");
				     PPCAT(bf_diagnostic_print, BN_PREFIX)(c);
#endif
				   PPCAT(bigfloat_normalize, BN_PREFIX)(c);
				   return;
			   }
		case LARGER:{
				    BF_IF_DIAGNOSTIC_INLINE(printf("add, larger\n");)
				    //need to shift b down
				    struct PPCAT(bf, BN_PREFIX) tmp;
				    struct PPCAT(bn_s, BN_PREFIX) exp_diff;
				    PPCAT(bignum_signed_sub, BN_PREFIX)(&a->exponent, &b->exponent, &exp_diff);
				    //TODO: check to make sure exp_diff can be stored in a uint64_t
				    uint64_t diff = PPCAT(bignum_to_int, BN_PREFIX)(&exp_diff.value);
				    PPCAT(bigfloat_assign, BN_PREFIX)(&tmp, b);
				    PPCAT(bf_shiftEXP, BN_PREFIX)(&tmp, diff);
#ifdef BF_DIAGNOSTICS
				    printf("post shift:  ");
				    PPCAT(bf_diagnostic_print, BN_PREFIX)(&tmp);
				    printf("a: ");
				    PPCAT(bf_diagnostic_print, BN_PREFIX)(a);
#endif
				    PPCAT(bignum_signed_add, BN_PREFIX)(&a->mantissa, &tmp.mantissa, &c->mantissa);
				    PPCAT(bignum_signed_assign, BN_PREFIX)(&c->exponent, &a->exponent);
				    PPCAT(bigfloat_normalize, BN_PREFIX)(c);
				    return;
			    }
		case SMALLER:{
				     BF_IF_DIAGNOSTIC_INLINE(printf("add, smaller\n");)
				     //need to shift a down
				     struct PPCAT(bn_s, BN_PREFIX) exp_diff;
				     struct PPCAT(bf, BN_PREFIX) tmp;
				     PPCAT(bignum_signed_sub, BN_PREFIX)(&b->exponent, &a->exponent, &exp_diff);
				     //TODO: check to make sure exp_diff can be stored in a uint64_t
				     uint64_t diff = PPCAT(bignum_to_int, BN_PREFIX)(&exp_diff.value);
				     PPCAT(bigfloat_assign, BN_PREFIX)(&tmp, a);
				     //printf("Exp diff: %lu\n", diff);
				     PPCAT(bf_shiftEXP, BN_PREFIX)(&tmp, diff);
#ifdef BF_DIAGNOSTICS
				     printf("post shift:  ");
				     PPCAT(bf_diagnostic_print, BN_PREFIX)(&tmp);
				     printf("b: ");
				     PPCAT(bf_diagnostic_print, BN_PREFIX)(b);
#endif
				     PPCAT(bignum_signed_add, BN_PREFIX)(&tmp.mantissa, &b->mantissa, &c->mantissa);
				     PPCAT(bignum_signed_assign, BN_PREFIX)(&c->exponent, &b->exponent);
				     PPCAT(bigfloat_normalize, BN_PREFIX)(c);
				     return;
			    }
	}
}

void PPCAT(bigfloat_sub, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* c){
	struct PPCAT(bf, BN_PREFIX) tmp;
	PPCAT(bigfloat_assign, BN_PREFIX)(&tmp, b);
	if(tmp.mantissa.sign == 0){
		tmp.mantissa.sign = 1;
	}else{
		tmp.mantissa.sign = 0;
	}
	PPCAT(bigfloat_add, BN_PREFIX)(a, &tmp, c);
}

#if !defined(BN_NO_DOUBLE_P) && BN_ARRAY_SIZE_MOD == 1

//#warning using double mul/div
//defined in the second pass with double precision
//should always be defined as long as BN_NO_DOUBLE_P is not defined
void bigfloat_change_exponent_2(struct bf_2* n , int wantedDigits);
int bf_get_maxDigits_2();

void bigfloat_mul(BN_VAR_PREFIX struct bf* a, BN_VAR_PREFIX struct bf* b, BN_VAR_PREFIX struct bf* c){
	//check the sign, and if one is negative subtract that one
	struct bf_2 atmp;
	struct bf_2 btmp;
	struct bf_2 ctmp;
	bigfloat_to_2(&atmp, a);
	bigfloat_to_2(&btmp, b);
	//rather than normalizing both a and b at the end (in addition to losing the truncated data), storing a temp var is better.
	int maxDigits = bf_get_maxDigits_2();
	bigfloat_change_exponent_2(&atmp, maxDigits/2 - 1);
	bigfloat_change_exponent_2(&btmp, maxDigits/2 - 1);
#ifdef BF_DIAGNOSTICS
	//printf("atmp:  ");
	//PPCAT(bf_diagnostic_print, BN_PREFIX)(&atmp);
	//printf("btmp:  ");
	//PPCAT(bf_diagnostic_print, BN_PREFIX)(&btmp);
#endif
	bignum_signed_add_2(&atmp.exponent, &btmp.exponent, &ctmp.exponent);
	bignum_signed_mul_2(&atmp.mantissa, &btmp.mantissa, &ctmp.mantissa);
	bigfloat_normalize_2(&ctmp);
	bigfloat_2_to_1(c, &ctmp);
	bigfloat_normalize(c);
}

void bigfloat_div(BN_VAR_PREFIX struct bf* a, BN_VAR_PREFIX struct bf* b, BN_VAR_PREFIX struct bf* c){
	if(PPCAT(bigfloat_is_zero, BN_PREFIX)(b)){
		assert(0);
		PPCAT(bigfloat_init, BN_PREFIX)(c);
		return;
	}

	//check the sign, and if one is negative subtract that one
	struct bf_2 atmp;
	struct bf_2 btmp;
	struct bf_2 ctmp;
	bigfloat_to_2(&atmp, a);
	bigfloat_to_2(&btmp, b);
	//rather than normalizing both a and b at the end (in addition to losing the truncated data), storing a temp var is better.
	int maxDigits = bf_get_maxDigits_2();
	//bigfloat_change_exponent_2(&atmp, maxDigits/2 - 1);
	bigfloat_change_exponent_2(&btmp, maxDigits/2 - 1);
#ifdef BF_DIAGNOSTICS
	//printf("atmp:  ");
	//PPCAT(bf_diagnostic_print, BN_PREFIX)(&atmp);
	//printf("btmp:  ");
	//PPCAT(bf_diagnostic_print, BN_PREFIX)(&btmp);
#endif
	bignum_signed_sub_2(&atmp.exponent, &btmp.exponent, &ctmp.exponent);
	bignum_signed_div_2(&atmp.mantissa, &btmp.mantissa, &ctmp.mantissa);
	bigfloat_normalize_2(&ctmp);
	bigfloat_2_to_1(c, &ctmp);
	bigfloat_normalize(c);
}

#else
void PPCAT(bigfloat_mul, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* c){
	//check the sign, and if one is negative subtract that one
	struct PPCAT(bf, BN_PREFIX) atmp;
	struct PPCAT(bf, BN_PREFIX) btmp;
	PPCAT(bigfloat_assign, BN_PREFIX)(&atmp, a);
	PPCAT(bigfloat_assign, BN_PREFIX)(&btmp, b);
	//rather than normalizing both a and b at the end (in addition to losing the truncated data), storing a temp var is better.
	int maxDigits = PPCAT(bf_get_maxDigits, BN_PREFIX)();
	PPCAT(bigfloat_change_exponent, BN_PREFIX)(&atmp, maxDigits/2 - 1);
	PPCAT(bigfloat_change_exponent, BN_PREFIX)(&btmp, maxDigits/2 - 1);
#ifdef BF_DIAGNOSTICS
	printf("atmp:  ");
	PPCAT(bf_diagnostic_print, BN_PREFIX)(&atmp);
	printf("btmp:  ");
	PPCAT(bf_diagnostic_print, BN_PREFIX)(&btmp);
#endif
	PPCAT(bignum_signed_add, BN_PREFIX)(&atmp.exponent, &btmp.exponent, &c->exponent);
	PPCAT(bignum_signed_mul, BN_PREFIX)(&atmp.mantissa, &btmp.mantissa, &c->mantissa);
	PPCAT(bigfloat_normalize, BN_PREFIX)(c);
}

void PPCAT(bigfloat_div, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* c){
	if(PPCAT(bigfloat_is_zero, BN_PREFIX)(b)){
		assert(0);
		PPCAT(bigfloat_init, BN_PREFIX)(c);
		return;
	}
	//check the sign, and if one is negative subtract that one
	struct PPCAT(bf, BN_PREFIX) atmp;
	struct PPCAT(bf, BN_PREFIX) btmp;
	PPCAT(bigfloat_assign, BN_PREFIX)(&atmp, a);
	PPCAT(bigfloat_assign, BN_PREFIX)(&btmp, b);
	//these are not neccesary for division, as a/b is always less digits than a (at least for intagers)
	int maxDigits = PPCAT(bf_get_maxDigits, BN_PREFIX)();
	//PPCAT(bigfloat_change_exponent, BN_PREFIX)(&atmp, maxDigits/2 - 1);
	PPCAT(bigfloat_change_exponent, BN_PREFIX)(&btmp, maxDigits/2 - 1);
#ifdef BF_DIAGNOSTICS
	printf("atmp:  ");
	PPCAT(bf_diagnostic_print, BN_PREFIX)(&atmp);
	printf("btmp:  ");
	PPCAT(bf_diagnostic_print, BN_PREFIX)(&btmp);
#endif
	PPCAT(bignum_signed_sub, BN_PREFIX)(&atmp.exponent, &btmp.exponent, &c->exponent);//add & mul -> sub & div
	PPCAT(bignum_signed_div, BN_PREFIX)(&atmp.mantissa, &btmp.mantissa, &c->mantissa);
#ifdef BF_DIAGNOSTICS
	printf("pre normalization:  ");
	PPCAT(bf_diagnostic_print, BN_PREFIX)(c);
#endif
	PPCAT(bigfloat_normalize, BN_PREFIX)(c);
}

#endif

int PPCAT(bigfloat_cmp, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* b){
#ifdef BF_DIAGNOSTICS
	printf("a: ");
	PPCAT(bf_diagnostic_print, BN_PREFIX)(a);
	printf("b: ");
	PPCAT(bf_diagnostic_print, BN_PREFIX)(b);
#endif
	if(PPCAT(bigfloat_is_zero, BN_PREFIX)(a)){
		BF_IF_DIAGNOSTIC_INLINE(printf("PPCAT(bigfloat_cmp, BN_PREFIX): a=0\n");)
		if(PPCAT(bigfloat_is_zero, BN_PREFIX)(b)){
			return(EQUAL);
		}else{
			if(b->mantissa.sign == 0){
				return(SMALLER);
			}else{
				return(LARGER);
			}
		}
	}else if(PPCAT(bigfloat_is_zero, BN_PREFIX)(b)){
		BF_IF_DIAGNOSTIC_INLINE(printf("PPCAT(bigfloat_cmp, BN_PREFIX): b=0\n");)
		if(PPCAT(bigfloat_is_zero, BN_PREFIX)(a)){
			return(EQUAL);
		}else{
			if(a->mantissa.sign == 0){
				return(LARGER);
			}else{
				return(SMALLER);
			}
		}
	}else{
		if(a->mantissa.sign && !b->mantissa.sign){
			BF_IF_DIAGNOSTIC_INLINE(printf("a is negative b is positive\n");)
			return(SMALLER);
		}
		if(!a->mantissa.sign && b->mantissa.sign){
			BF_IF_DIAGNOSTIC_INLINE(printf("a is negative b is positive\n");)
			return(LARGER);
		}
		int exp_r = PPCAT(bignum_signed_cmp, BN_PREFIX)(&a->exponent, &b->exponent);
		if(exp_r == EQUAL){
			int man_r = PPCAT(bignum_signed_cmp, BN_PREFIX)(&a->mantissa, &b->mantissa);
			return(man_r);
		}else{
			if(a->mantissa.sign && b->mantissa.sign){
				switch(exp_r){
					case LARGER:
						return SMALLER;
					case SMALLER:
						return LARGER;
					default:
						return EQUAL;
				}
			}else{
				return exp_r;
			}
		}
	}
}

void PPCAT(bigfloat_assign, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* dst, BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* src){
	PPCAT(bignum_signed_assign, BN_PREFIX)(&dst->exponent, &src->exponent);
	PPCAT(bignum_signed_assign, BN_PREFIX)(&dst->mantissa, &src->mantissa);
}

int PPCAT(bigfloat_is_zero, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n){
	return(PPCAT(bignum_signed_is_zero, BN_PREFIX)(&n->mantissa));//exponent can't change zero :/
}

#if BF_BASE == 2
void PPCAT(bf_shiftEXP, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n, int shift){
	if(PPCAT(bigfloat_is_zero, BN_PREFIX)(n)){
		PPCAT(bigfloat_init, BN_PREFIX)(n);
		return;
	}
	if(shift == 0){
		return;
	}
	if(shift < 0){
		BF_IF_DIAGNOSTIC_INLINE(printf("shift less than 0\n");)
		int bshift = -shift;
		struct PPCAT(bn, BN_PREFIX) s_tmp;
		PPCAT(bignum_lshift, BN_PREFIX)(&n->mantissa.value, &s_tmp, bshift);
		PPCAT(bignum_assign, BN_PREFIX)(&n->mantissa.value, &s_tmp);
		struct PPCAT(bn_s, BN_PREFIX) e_tmp;
		struct PPCAT(bn_s, BN_PREFIX) i_tmp;
		PPCAT(bignum_signed_from_int, BN_PREFIX)(&i_tmp, shift);
		PPCAT(bignum_signed_add, BN_PREFIX)(&n->exponent, &i_tmp, &e_tmp);
		PPCAT(bignum_signed_assign, BN_PREFIX)(&n->exponent, &e_tmp);
	}else{
		BF_IF_DIAGNOSTIC_INLINE(printf("shift greater than 0\n");)
		//shift < 0
		int bshift = shift;
		struct PPCAT(bn, BN_PREFIX) s_tmp;
		PPCAT(bignum_rshift, BN_PREFIX)(&n->mantissa.value, &s_tmp, bshift);
		PPCAT(bignum_assign, BN_PREFIX)(&n->mantissa.value, &s_tmp);
		struct PPCAT(bn_s, BN_PREFIX) e_tmp;
		struct PPCAT(bn_s, BN_PREFIX) i_tmp;
		PPCAT(bignum_signed_from_int, BN_PREFIX)(&i_tmp, shift);
		PPCAT(bignum_signed_add, BN_PREFIX)(&n->exponent, &i_tmp, &e_tmp);
		PPCAT(bignum_signed_assign, BN_PREFIX)(&n->exponent, &e_tmp);
	}
}
unsigned int PPCAT(numPlaces, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n){
	return((BN_BITS * BN_ARRAY_SIZE_MOD) - PPCAT(bignum_bsr, BN_PREFIX)(n));
}
int PPCAT(bf_get_maxDigits, BN_PREFIX)(){
	return((BN_BYTES * BN_ARRAY_SIZE_MOD) * 8 - 1);//that was easy
}
#else
void PPCAT(bf_shiftEXP, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n, int shift){
	if(PPCAT(bigfloat_is_zero, BN_PREFIX)(n)){
		PPCAT(bigfloat_init, BN_PREFIX)(n);
		return;
	}
	if(shift == 0){
		return;
	}
	struct PPCAT(bn_s, BN_PREFIX) tmp;
	struct PPCAT(bn_s, BN_PREFIX) tmp_e;
	struct PPCAT(bn, BN_PREFIX) tmp_m;
	struct PPCAT(bn, BN_PREFIX) baseNum;
	struct PPCAT(bn, BN_PREFIX) tmp2;
	PPCAT(bignum_signed_from_int, BN_PREFIX)(&tmp, shift);
	PPCAT(bignum_from_int, BN_PREFIX)(&baseNum, BF_BASE);
	PPCAT(bignum_pow, BN_PREFIX)(&baseNum, &tmp.value, &tmp2);//sets tmp2 to the base times the number of shifts
	if(shift < 0){
		PPCAT(bignum_mul, BN_PREFIX)(&n->mantissa.value, &tmp2, &tmp_m);
	}else{
		PPCAT(bignum_div, BN_PREFIX)(&n->mantissa.value, &tmp2, &tmp_m);	
	}
	PPCAT(bignum_assign, BN_PREFIX)(&n->mantissa.value, &tmp_m);


	PPCAT(bignum_signed_add, BN_PREFIX)(&n->exponent, &tmp, &tmp_e);
	PPCAT(bignum_signed_assign, BN_PREFIX)(&n->exponent, &tmp_e);
}
unsigned int PPCAT(numPlaces, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* n){
	if(PPCAT(bignum_is_zero, BN_PREFIX)(n)){
		return(PPCAT(bf_get_maxDigits, BN_PREFIX)());
	}
	unsigned int r = 0;
	struct PPCAT(bn, BN_PREFIX) tmp;
	struct PPCAT(bn, BN_PREFIX) baseNum;
	PPCAT(bignum_assign, BN_PREFIX)(&tmp, n);
	PPCAT(bignum_from_int, BN_PREFIX)(&baseNum, BF_BASE);
	while(1){
		int result = PPCAT(bignum_is_zero, BN_PREFIX)(&tmp);
		if(!result){
			struct PPCAT(bn, BN_PREFIX) n_tmp;
			PPCAT(bignum_assign, BN_PREFIX)(&n_tmp, &tmp);
			PPCAT(bignum_div, BN_PREFIX)(&n_tmp, &baseNum, &tmp);
			r++;
		}else{
			break;
		}
	}
	return(r);
}

int PPCAT(bf_get_maxDigits, BN_PREFIX)(){
//    bn max;
//	bignum_init(&max);
//	bignum_dec(&max);
//	int maxDigits = PPCAT(numPlaces, BN_PREFIX)(&max);
//	int result1 = maxDigits;

    //double log2 = 0.30102999566398119521373889472449302676818988146210854131042746112710818927;
    double log2 = log(2)/log(BF_BASE);
    double size = 8 * (BN_BYTES * BN_ARRAY_SIZE_MOD);
    double result = log2 * size;
    int result2 = ((int)result) + 1;

//    assert(result1 == result2);
    return result2;
}
#endif

void PPCAT(bigfloat_change_exponent, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n, int wantedDigits){
	//int maxDigits = PPCAT(bf_get_maxDigits, BN_PREFIX)();
	int currentNumDigits = PPCAT(numPlaces, BN_PREFIX)(&n->mantissa.value);
	int digDiff = wantedDigits - currentNumDigits;
	int digShift = -(digDiff);
	//printf("current digits: %i, digShift: %i, digDiff: %i (wantedDigits=%i)\n", currentNumDigits, digShift, digDiff, wantedDigits);
	PPCAT(bf_shiftEXP, BN_PREFIX)(n, digShift);
	//int n_currentNumDigits = PPCAT(numPlaces, BN_PREFIX)(&n->mantissa.value);
	//printf("n_currentNumDigits: %i, exp: %i\n", n_currentNumDigits, PPCAT(bignum_signed_to_int, BN_PREFIX)(&n->exponent));
}

void PPCAT(bigfloat_normalize, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n){
	//should leave it so that there is one place blank in mantissa, and adjust the exponent accordingly
	PPCAT(bigfloat_change_exponent, BN_PREFIX)(n, PPCAT(bf_get_maxDigits, BN_PREFIX)() - 2);
}


void PPCAT(bigfloat_inc, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n){
	struct PPCAT(bf, BN_PREFIX) one;
	struct PPCAT(bf, BN_PREFIX) tmp;
	PPCAT(bigfloat_from_int, BN_PREFIX)(&one, 1);
	PPCAT(bigfloat_add, BN_PREFIX)(n, &one, &tmp);
	PPCAT(bigfloat_assign, BN_PREFIX)(n, &tmp);
	//bignum_signed_inc(&n->mantissa);
	//PPCAT(bigfloat_normalize, BN_PREFIX)(n);
}

void PPCAT(bigfloat_dec, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bf, BN_PREFIX)* n){
	struct PPCAT(bf, BN_PREFIX) one;
	struct PPCAT(bf, BN_PREFIX) tmp;
	PPCAT(bigfloat_from_int, BN_PREFIX)(&one, 1);
	PPCAT(bigfloat_sub, BN_PREFIX)(n, &one, &tmp);
	PPCAT(bigfloat_assign, BN_PREFIX)(n, &tmp);
	//bignum_signed_dec(&n->mantissa);
	//PPCAT(bigfloat_normalize, BN_PREFIX)(n);
}
