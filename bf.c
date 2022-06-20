#include <stdio.h>
#include "bf.h"

void bigfloat_change_exponent(struct bf* n, int wantedDigits);
int bf_get_maxDigits();
void bf_shiftEXP(struct bf* n, int shift);
unsigned int numPlaces(struct bn* n);

void bigfloat_init(struct bf* n){
	bignum_signed_init(&n->exponent);
	bignum_signed_init(&n->mantissa);
}

void bigfloat_from_bn(struct bf* n, struct bn* bigint){
	bignum_signed_from_bn(&n->mantissa, bigint);
	bignum_signed_from_int(&n->exponent, 0);
	bigfloat_normalize(n);
}

void bigfloat_from_bn_s(struct bf* n, struct bn_s* bigint_signed){
	bignum_signed_assign(&n->mantissa, bigint_signed);
	bignum_signed_from_int(&n->exponent, 0);
	bigfloat_normalize(n);
}

void bigfloat_from_int(struct bf* n, int64_t value){
	bignum_signed_from_int(&n->mantissa, value);
	bignum_signed_from_int(&n->exponent, 0);
	bigfloat_normalize(n);
}


typedef union {
  double f;
  struct {
    uint64_t mantisa : 52;
    int exponent : 11;
    unsigned int sign : 1;
  } parts;
} float_cast;

void bigfloat_from_double(struct bf* n, double d){
	float_cast d1;
	d1.f = d;
	uint64_t mantissa = d1.parts.mantisa;
	int64_t exponent = d1.parts.exponent;
	int64_t mantissa_s;
	if(d1.parts.sign != 0){
		mantissa_s = -((int)mantissa);
	}else{
		mantissa_s = (int)mantissa;
	}
	bignum_signed_from_int(&n->exponent, exponent);
	bignum_signed_from_int(&n->mantissa, mantissa);
	bigfloat_normalize(n);
}

double bigfloat_to_double(struct bf* n){
	/*
	float_cast d1;
	struct bf tmp;
	bigfloat_assign(&tmp, n);
	//from here we need to shift the mantissa and add/sub to the exponent so that the mantissa just fits in a double
	uint64_t mantissa = bignum_to_int(&tmp.mantissa);
	int64_t exponent = bignum_to_int(&tmp.exponent);
	if(n->exponent_sign){
		exponent = -exponent;
	}
	d1.parts.sign = n->mantissa_sign;
	d1.parts.exponent = exponent;
	d1.parts.mantisa = mantissa;
	return(d1.f);
	*/
	return 0;
}


//based off of https://en.wikipedia.org/wiki/Floating-point_arithmetic#Floating-point_operations

void bigfloat_add(struct bf* a, struct bf* b, struct bf* c){
	int expdif = bignum_signed_cmp(&a->exponent, &b->exponent);
	switch(expdif){
		case EQUAL:{
				   //printf("add, equal\n");
				   //just add the mantissa
				   bignum_signed_add(&a->mantissa, &b->mantissa, &c->mantissa);
				   bignum_signed_assign(&c->exponent, &a->exponent);
				   bigfloat_normalize(c);
				   return;
			   }
		case LARGER:{
				    //printf("add, larger\n");
				    //need to shift b down
				    struct bf tmp;
				    struct bn_s exp_diff;
				    bignum_signed_sub(&a->exponent, &b->exponent, &exp_diff);
				    //TODO: check to make sure exp_diff can be stored in a uint64_t
				    uint64_t diff = bignum_to_int(&exp_diff.value);
				    bigfloat_assign(&tmp, b);
				    bf_shiftEXP(&tmp, diff);
				    bignum_signed_add(&a->mantissa, &tmp.mantissa, &c->mantissa);
				    bignum_signed_assign(&c->exponent, &a->exponent);
				    bigfloat_normalize(c);
				    return;
			    }
		case SMALLER:{
				     //printf("add, smaller\n");
				     //need to shift a down
				     struct bn_s exp_diff;
				     struct bf tmp;
				     bignum_signed_sub(&b->exponent, &a->exponent, &exp_diff);
				     //TODO: check to make sure exp_diff can be stored in a uint64_t
				     uint64_t diff = bignum_to_int(&exp_diff.value);
				     bigfloat_assign(&tmp, a);
				     //printf("Exp diff: %lu\n", diff);
				     bf_shiftEXP(&tmp, diff);
				     bignum_signed_add(&tmp.mantissa, &b->mantissa, &c->mantissa);
				     bignum_signed_assign(&c->exponent, &b->exponent);
				     bigfloat_normalize(c);
				     return;
			    }
	}
}

void bigfloat_mul(struct bf* a, struct bf* b, struct bf* c){
	//check the sign, and if one is negative subtract that one
	struct bf atmp;
	struct bf btmp;
	bigfloat_assign(&atmp, a);
	bigfloat_assign(&btmp, b);
	//rather than normalizing both a and b at the end (in addition to losing the truncated data), storing a temp var is better.
	int maxDigits = bf_get_maxDigits();
	bigfloat_change_exponent(&atmp, maxDigits/2 + 1);
	bigfloat_change_exponent(&btmp, maxDigits/2 + 1);
	bignum_signed_add(&atmp.exponent, &btmp.exponent, &c->exponent);
	bignum_signed_mul(&atmp.mantissa, &btmp.mantissa, &c->mantissa);
	bigfloat_normalize(c);
}

int bigfloat_cmp(struct bf* a, struct bf* b){
	int exp_r = bignum_signed_cmp(&a->exponent, &b->exponent);
	if(exp_r == EQUAL){
		int man_r = bignum_signed_cmp(&a->mantissa, &b->mantissa);
		return(man_r);
	}else{
		return exp_r;
	}
}

void bigfloat_assign(struct bf* dst, struct bf* src){
	bignum_signed_assign(&dst->exponent, &src->exponent);
	bignum_signed_assign(&dst->mantissa, &src->mantissa);
}

int bigfloat_is_zero(struct bf* n){
	return(bignum_signed_is_zero(&n->mantissa));//exponent can't change zero :/
}

#if BF_BASE == 2
void bf_shiftEXP(struct bf* n, int shift){
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
unsigned int numPlaces(struct bn* n){
	return(bignum_bsr(n));
}
#else
void bf_shiftEXP(struct bf* n, int shift){
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
unsigned int numPlaces(struct bn* n){
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

    double log2 = 0.301029995664;
    double size = 8 * BN_BYTES;
    double result = log2 * size;
    int result2 = ((int)result) + 1;

//    assert(result1 == result2);
    return result2;
}

void bigfloat_change_exponent(struct bf* n, int wantedDigits){
	int maxDigits = bf_get_maxDigits();
	int currentNumDigits = numPlaces(&n->mantissa.value);
	int digDiff = -(maxDigits - wantedDigits - currentNumDigits);
	//printf("current digits: %i, changing by %i\n", currentNumDigits, digDiff);
	bf_shiftEXP(n, digDiff);
}

void bigfloat_normalize(struct bf* n){
	//should leave it so that there is one place blank in mantissa, and adjust the exponent accordingly
	bigfloat_change_exponent(n, 1);
}

/*
 * TODO:
 * figure out how the normalize function should work
 * code it.
 * fix add.
 */
