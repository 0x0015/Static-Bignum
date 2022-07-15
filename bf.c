#include "bf.h"
#ifdef BF_DIAGNOSTICS
#include <stdio.h>

void bf_diagnostic_print(BN_VAR_PREFIX struct bf* n){
	double d = bigfloat_to_double(n);
	char buf1[1000];
	char buf2[1000];
	int64_t man = bignum_signed_to_int(&n->mantissa);
	int64_t exp = bignum_signed_to_int(&n->exponent);
	bignum_signed_to_string(&n->mantissa, buf1, 1000);
	bignum_signed_to_string(&n->exponent, buf2, 1000);
	printf("mantissa: %s (%li) exponent: %s (%li)   (%lf)\n", buf1, man, buf2, exp, d);
}
#define BF_IF_DIAGNOSTIC_INLINE(x) x
#else
#define BF_IF_DIAGNOSTIC_INLINE(x) 
#endif

void bigfloat_change_exponent(BN_VAR_PREFIX struct bf* n, int wantedDigits);
int bf_get_maxDigits();
void bf_shiftEXP(BN_VAR_PREFIX struct bf* n, int shift);
unsigned int numPlaces(BN_VAR_PREFIX struct bn* n);

void bigfloat_init(BN_VAR_PREFIX struct bf* n){
	bignum_signed_init(&n->exponent);
	bignum_signed_init(&n->mantissa);
}

void bigfloat_from_bn(BN_VAR_PREFIX struct bf* n, BN_VAR_PREFIX struct bn* bigint){
	bignum_signed_from_bn(&n->mantissa, bigint);
	bignum_signed_from_int(&n->exponent, 0);
	bigfloat_normalize(n);
}

void bigfloat_from_bn_s(BN_VAR_PREFIX struct bf* n, BN_VAR_PREFIX struct bn_s* bigint_signed){
	bignum_signed_assign(&n->mantissa, bigint_signed);
	bignum_signed_from_int(&n->exponent, 0);
	bigfloat_normalize(n);
}

void bigfloat_from_int(BN_VAR_PREFIX struct bf* n, int64_t value){
	bignum_signed_from_int(&n->mantissa, value);
	bignum_signed_from_int(&n->exponent, 0);
	bigfloat_normalize(n);
}


#ifndef BN_NO_STDMATH
#include <math.h>
#else
#ifdef BN_CUSTOM_FMATH
//from https://stackoverflow.com/questions/70382318/ex-without-math-h
double fabs(double x) {
    if(x >= 0){
        return x;
    } else {
        return -x;
    }
}
double exp(double x) {
    double a = 1.0, e = 0;
    int invert = x<0;
    x = fabs(x);
    for (int n = 1; e != e + a ; ++n) {
        e += a;
        a = a * x / n;
    }
    return invert ? 1/e : e;
}
//from https://stackoverflow.com/a/64896598
#define EULER_CONST 2.718281828459045235
#define TAYLOR_ITERATIONS 20
double log(double x) {
    // Trap illegal values
    if (x <= 0) {
        return 0.0/0.0;  // NaN
    }
    
    // Confine x to a sensible range
    int power_adjust = 0;
    while (x > 1.0) {
        x /= EULER_CONST;
        power_adjust++;
    }
    while (x < .25) {
        x *= EULER_CONST;
        power_adjust--;
    }
    
    // Now use the Taylor series to calculate the logarithm
    x -= 1.0;
    double t = 0.0, s = 1.0, z = x;
    for (int k=1; k<=TAYLOR_ITERATIONS; k++) {
        t += z * s / k;
        z *= x;
        s = -s;
    }
    
    // Combine the result with the power_adjust value and return
    return t + power_adjust;
}
//from https://stackoverflow.com/a/14104864
double pow(double x, double y) {
    return exp(log(x)*y);
}
double fmax(double a, double b){
	if(a > b){
		return(a);
	}else{
		return(b);
	}
}
#endif
#endif

#if (!defined(BN_NO_STDMATH)) || defined(BN_CUSTOM_FMATH) || defined(BN_NATIVE_FMATH)
#define BF_EPSILON 10e-12
int bf_closeWithinEpsilon(double d1, double d2){
	return (fabs(d1-d2) < BF_EPSILON * fmax(fabs(d1), fabs(d2))); //epsilon is 0.1 for now
}
void bigfloat_from_double(BN_VAR_PREFIX struct bf* n, double d){
	double rd = log(d)/log(BF_BASE);//how many powers of bf_base fit in d?
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
		//printf("multiple of BF_BASE\n");
		er--;
	}
	//printf("r=%i (rd=%lf)\n", r, rd);
	double rDIV = d / pow(BF_BASE, r);
	unsigned int rmax = log(MAX_VAL)/log(BF_BASE);
	int64_t mres = rDIV * pow(BF_BASE, rmax);
	unsigned int mresr = log(mres)/log(BF_BASE);
	int64_t exp = -mresr;
	bignum_signed_from_int(&n->mantissa, mres);
	bignum_signed_from_int(&n->exponent, exp+r+er);
	bigfloat_normalize(n);
}
double bigfloat_to_double(BN_VAR_PREFIX struct bf* n){
	struct bf tmp;
	bigfloat_assign(&tmp, n); 
	unsigned int rmax = log(MAX_VAL)/log(BF_BASE);
	bigfloat_change_exponent(&tmp, rmax);
	int64_t man = bignum_signed_to_int(&tmp.mantissa);
	int64_t exp = bignum_signed_to_int(&tmp.exponent);
	unsigned int r = log(man)/log(BF_BASE);
	//printf("man: %li, exp: %li\n", man, exp);
	double output = man;
	output *= pow(BF_BASE, exp);
	return(output);
}
#endif

//based off of https://en.wikipedia.org/wiki/Floating-point_arithmetic#Floating-point_operations

void bigfloat_add(BN_VAR_PREFIX struct bf* a, BN_VAR_PREFIX struct bf* b, BN_VAR_PREFIX struct bf* c){
	int expdif = bignum_signed_cmp(&a->exponent, &b->exponent);
	switch(expdif){
		case EQUAL:{
				   BF_IF_DIAGNOSTIC_INLINE(printf("add, equal\n");)
				   //just add the mantissa
#ifdef BF_DIAGNOSTICS
				     printf("a:  ");
				     bf_diagnostic_print(a);
				     printf("b:  ");
				     bf_diagnostic_print(b);
#endif
				   //bf_shiftEXP(a, 1);
				   //bf_shiftEXP(b, 1);
				   bignum_signed_add(&a->mantissa, &b->mantissa, &c->mantissa);
				   bignum_signed_assign(&c->exponent, &a->exponent);
#ifdef BF_DIAGNOSTICS
				     printf("pre normalization:  ");
				     bf_diagnostic_print(c);
#endif
				   bigfloat_normalize(c);
				   return;
			   }
		case LARGER:{
				    BF_IF_DIAGNOSTIC_INLINE(printf("add, larger\n");)
				    //need to shift b down
				    struct bf tmp;
				    struct bn_s exp_diff;
				    bignum_signed_sub(&a->exponent, &b->exponent, &exp_diff);
				    //TODO: check to make sure exp_diff can be stored in a uint64_t
				    uint64_t diff = bignum_to_int(&exp_diff.value);
				    bigfloat_assign(&tmp, b);
				    bf_shiftEXP(&tmp, diff);
#ifdef BF_DIAGNOSTICS
				    printf("post shift:  ");
				    bf_diagnostic_print(&tmp);
				    printf("a: ");
				    bf_diagnostic_print(a);
#endif
				    bignum_signed_add(&a->mantissa, &tmp.mantissa, &c->mantissa);
				    bignum_signed_assign(&c->exponent, &a->exponent);
				    bigfloat_normalize(c);
				    return;
			    }
		case SMALLER:{
				     BF_IF_DIAGNOSTIC_INLINE(printf("add, smaller\n");)
				     //need to shift a down
				     struct bn_s exp_diff;
				     struct bf tmp;
				     bignum_signed_sub(&b->exponent, &a->exponent, &exp_diff);
				     //TODO: check to make sure exp_diff can be stored in a uint64_t
				     uint64_t diff = bignum_to_int(&exp_diff.value);
				     bigfloat_assign(&tmp, a);
				     //printf("Exp diff: %lu\n", diff);
				     bf_shiftEXP(&tmp, diff);
#ifdef BF_DIAGNOSTICS
				     printf("post shift:  ");
				     bf_diagnostic_print(&tmp);
				     printf("b: ");
				     bf_diagnostic_print(b);
#endif
				     bignum_signed_add(&tmp.mantissa, &b->mantissa, &c->mantissa);
				     bignum_signed_assign(&c->exponent, &b->exponent);
				     bigfloat_normalize(c);
				     return;
			    }
	}
}

void bigfloat_sub(BN_VAR_PREFIX struct bf* a, BN_VAR_PREFIX struct bf* b, BN_VAR_PREFIX struct bf* c){
	struct bf tmp;
	bigfloat_assign(&tmp, b);
	if(tmp.mantissa.sign == 0){
		tmp.mantissa.sign = 1;
	}else{
		tmp.mantissa.sign = 0;
	}
	bigfloat_add(a, &tmp, c);
}

void bigfloat_mul(BN_VAR_PREFIX struct bf* a, BN_VAR_PREFIX struct bf* b, BN_VAR_PREFIX struct bf* c){
	//check the sign, and if one is negative subtract that one
	struct bf atmp;
	struct bf btmp;
	bigfloat_assign(&atmp, a);
	bigfloat_assign(&btmp, b);
	//rather than normalizing both a and b at the end (in addition to losing the truncated data), storing a temp var is better.
	int maxDigits = bf_get_maxDigits();
	bigfloat_change_exponent(&atmp, maxDigits/2 - 1);
	bigfloat_change_exponent(&btmp, maxDigits/2 - 1);
#ifdef BF_DIAGNOSTICS
	printf("atmp:  ");
	bf_diagnostic_print(&atmp);
	printf("btmp:  ");
	bf_diagnostic_print(&btmp);
#endif
	bignum_signed_add(&atmp.exponent, &btmp.exponent, &c->exponent);
	bignum_signed_mul(&atmp.mantissa, &btmp.mantissa, &c->mantissa);
	bigfloat_normalize(c);
}

void bigfloat_div(BN_VAR_PREFIX struct bf* a, BN_VAR_PREFIX struct bf* b, BN_VAR_PREFIX struct bf* c){
	if(bigfloat_is_zero(b)){
		assert(0);
		bigfloat_init(c);
		return;
	}
	//check the sign, and if one is negative subtract that one
	struct bf atmp;
	struct bf btmp;
	bigfloat_assign(&atmp, a);
	bigfloat_assign(&btmp, b);
	//these are not neccesary for division, as a/b is always less digits than a (at least for intagers)
	int maxDigits = bf_get_maxDigits();
	//bigfloat_change_exponent(&atmp, maxDigits/2 - 1);
	bigfloat_change_exponent(&btmp, maxDigits/2 - 1);
#ifdef BF_DIAGNOSTICS
	printf("atmp:  ");
	bf_diagnostic_print(&atmp);
	printf("btmp:  ");
	bf_diagnostic_print(&btmp);
#endif
	bignum_signed_sub(&atmp.exponent, &btmp.exponent, &c->exponent);//add & mul -> sub & div
	bignum_signed_div(&atmp.mantissa, &btmp.mantissa, &c->mantissa);
#ifdef BF_DIAGNOSTICS
	printf("pre normalization:  ");
	bf_diagnostic_print(c);
#endif
	bigfloat_normalize(c);
}

int bigfloat_cmp(BN_VAR_PREFIX struct bf* a, BN_VAR_PREFIX struct bf* b){
	if(bigfloat_is_zero(a)){
		if(bigfloat_is_zero(b)){
			return(EQUAL);
		}else{
			if(b->mantissa.sign == 0){
				return(SMALLER);
			}else{
				return(LARGER);
			}
		}
	}else if(bigfloat_is_zero(b)){
		if(bigfloat_is_zero(a)){
			return(EQUAL);
		}else{
			if(a->mantissa.sign == 0){
				return(LARGER);
			}else{
				return(SMALLER);
			}
		}
	}else{
		int exp_r = bignum_signed_cmp(&a->exponent, &b->exponent);
		if(exp_r == EQUAL){
			int man_r = bignum_signed_cmp(&a->mantissa, &b->mantissa);
			return(man_r);
		}else{
			return exp_r;
		}
	}
}

void bigfloat_assign(BN_VAR_PREFIX struct bf* dst, BN_VAR_PREFIX struct bf* src){
	bignum_signed_assign(&dst->exponent, &src->exponent);
	bignum_signed_assign(&dst->mantissa, &src->mantissa);
}

int bigfloat_is_zero(BN_VAR_PREFIX struct bf* n){
	return(bignum_signed_is_zero(&n->mantissa));//exponent can't change zero :/
}

#if BF_BASE == 2
void bf_shiftEXP(BN_VAR_PREFIX struct bf* n, int shift){
	if(bigfloat_is_zero(n)){
		bigfloat_init(n);
		return;
	}
	if(shift == 0){
		return;
	}
	if(shift > 0){
		struct bn s_tmp;
		bignum_lshift(&n->mantissa.value, &s_tmp, shift);
		bignum_assign(&n->mantissa.value, &s_tmp);
		struct bn_s e_tmp;
		struct bn_s i_tmp;
		bignum_signed_from_int(&i_tmp, shift);
		bignum_signed_add(&n->exponent, &i_tmp, &e_tmp);
		bignum_signed_assign(&n->exponent, &e_tmp);
	}else{
		//shift < 0
		int bshift = -shift;
		struct bn s_tmp;
		bignum_rshift(&n->mantissa.value, &s_tmp, bshift);
		bignum_assign(&n->mantissa.value, &s_tmp);
		struct bn_s e_tmp;
		struct bn_s i_tmp;
		bignum_signed_from_int(&i_tmp, shift);
		bignum_signed_add(&n->exponent, &i_tmp, &e_tmp);
		bignum_signed_assign(&n->exponent, &e_tmp);
	}
}
unsigned int numPlaces(BN_VAR_PREFIX struct bn* n){
	return(bignum_bsr(n));
}
#else
void bf_shiftEXP(BN_VAR_PREFIX struct bf* n, int shift){
	if(bigfloat_is_zero(n)){
		bigfloat_init(n);
		return;
	}
	if(shift == 0){
		return;
	}
	struct bn_s tmp;
	struct bn_s tmp_e;
	struct bn tmp_m;
	struct bn baseNum;
	struct bn tmp2;
	bignum_signed_from_int(&tmp, shift);
	bignum_from_int(&baseNum, BF_BASE);
	bignum_pow(&baseNum, &tmp.value, &tmp2);//sets tmp2 to the base times the number of shifts
	if(shift < 0){
		bignum_mul(&n->mantissa.value, &tmp2, &tmp_m);
	}else{
		bignum_div(&n->mantissa.value, &tmp2, &tmp_m);	
	}
	bignum_assign(&n->mantissa.value, &tmp_m);


	bignum_signed_add(&n->exponent, &tmp, &tmp_e);
	bignum_signed_assign(&n->exponent, &tmp_e);
}
unsigned int numPlaces(BN_VAR_PREFIX struct bn* n){
	if(bignum_is_zero(n)){
		return(bf_get_maxDigits());
	}
	unsigned int r = 0;
	struct bn tmp;
	struct bn baseNum;
	bignum_assign(&tmp, n);
	bignum_from_int(&baseNum, BF_BASE);
	while(1){
		int result = bignum_is_zero(&tmp);
		if(!result){
			struct bn n_tmp;
			bignum_assign(&n_tmp, &tmp);
			bignum_div(&n_tmp, &baseNum, &tmp);
			r++;
		}else{
			break;
		}
	}
	return(r);
}
#endif

int bf_get_maxDigits(){
//    bn max;
//	bignum_init(&max);
//	bignum_dec(&max);
//	int maxDigits = numPlaces(&max);
//	int result1 = maxDigits;

    double log2 = 0.30102999566398119521373889472449302676818988146210854131042746112710818927;
    double size = 8 * BN_BYTES;
    double result = log2 * size;
    int result2 = ((int)result) + 1;

//    assert(result1 == result2);
    return result2;
}

void bigfloat_change_exponent(BN_VAR_PREFIX struct bf* n, int wantedDigits){
	int maxDigits = bf_get_maxDigits();
	int currentNumDigits = numPlaces(&n->mantissa.value);
	int digDiff = wantedDigits - currentNumDigits;
	int digShift = -(digDiff);
	//printf("current digits: %i, digShift: %i, digDiff: %i (wantedDigits=%i, maxDigits=%i)\n", currentNumDigits, digShift, digDiff, wantedDigits, maxDigits);
	bf_shiftEXP(n, digShift);
	int n_currentNumDigits = numPlaces(&n->mantissa.value);
	//printf("n_currentNumDigits: %i, exp: %i\n", n_currentNumDigits, bignum_signed_to_int(&n->exponent));
}

void bigfloat_normalize(BN_VAR_PREFIX struct bf* n){
	//should leave it so that there is one place blank in mantissa, and adjust the exponent accordingly
	bigfloat_change_exponent(n, bf_get_maxDigits() - 2);
}


void bigfloat_inc(BN_VAR_PREFIX struct bf* n){
	bignum_signed_inc(&n->mantissa);
	bigfloat_normalize(n);
}

void bigfloat_dec(BN_VAR_PREFIX struct bf* n){
	bignum_signed_dec(&n->mantissa);
	bigfloat_normalize(n);
}
