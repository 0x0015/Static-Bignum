#include "bf.h"

void bigfloat_init(struct bf* n){
	bignum_signed_init(&n->exponent);
	bignum_signed_init(&n->mantissa);
}

void bigfloat_from_bn(struct bf* n, struct bn* bigint){
	bignum_signed_from_bn(&n->mantissa, bigint);
	bignum_signed_from_int(&n->exponent, 0);
	bigfloat_normalize(n);
}

void bigfloat_from_int(struct bf* n, int64_t value){
	bignum_signed_from_int(&n->mantissa, value);
	bignum_signed_from_int(&n->exponent, 0);
	bigfloat_normalize(n);
}

/*
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
	bignum_from_int(&n->exponent, exponent);
	bignum_from_int(&n->mantissa, mantissa);
	n->mantissa_sign = d1.parts.sign;
	if(exponent < 0){
		n->exponent_sign = 1;
	}else{
		n->exponent_sign = 0;
	}
	bigfloat_normalize(n);
}

double bigfloat_to_double(struct bf* n){
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
}
*/

//based off of https://en.wikipedia.org/wiki/Floating-point_arithmetic#Floating-point_operations

void bigfloat_add_exp_same(struct bn_s* mt1, struct bn_s* bt2, struct bn_s* exp, struct bf* c){
	//the exponent is the same so the mantissa just needs to be added.  this is more complicated than it seems because we need to worry about overflow (eg 10+10 = 100 -> 10*10^0+10*10^0=10*10^1).
}

void bigfloat_add(struct bf* a, struct bf* b, struct bf* c){
	/*
	int expdif = bignum_signed_cmp(&a->exponent, &b->exponent);
	switch(expdif){
		case EQUAL:{
				   //just add the mantissa
				   bigfloat_add_exp_same(&a->mantissa, &b->mantissa, &a->exponent, c);
				   return;
			   }
		case LARGER:{
				    //need to shift b down
				    struct bn tmp_e;
				    struct bn tmp_m;
				    //struct bn tmp;
				    bignum_sub(&a->exponent, &b->exponent, &tmp_e);
				    //bignum_from_int(&tmp, BF_BASE);
				    //bignum_div(&b->mantissa, &tmp, &tmp_m);//mantissa should now be shifted so that the exponents match
				     int shift_bits = bignum_to_int(&tmp_e);
				     bignum_rshift(&b->mantissa, &tmp_m, shift_bits);
				    bigfloat_add_exp_same(&a->mantissa, &tmp_m, &a->exponent, c);
				    return;
			    }
		case SMALLER:{
				     //need to shift a down
				     struct bn tmp_e;
				     struct bn tmp_m;
				     //struct bn tmp;
				     bignum_sub(&b->exponent, &a->exponent, &tmp_e);
				     //bignum_from_int(&tmp, BF_BASE);
				     //bignum_div(&a->mantissa, &tmp, &tmp_m);//mantissa should now be shifted so that the exponents match
				     int shift_bits = bignum_to_int(&tmp_e);
				     bignum_rshift(&a->mantissa, &tmp_m, shift_bits);
				     bigfloat_add_exp_same(&b->mantissa, &tmp_m, &b->exponent, c);
				     return;
				     
			    }
			break;
	}
	*/
	//this will all get fixed once normalization is in place
}

void bigfloat_mul(struct bf* a, struct bf* b, struct bf* c){
	//check the sign, and if one is negative subtract that one
	bignum_signed_add(&a->exponent, &b->exponent, &c->exponent);
	bignum_signed_mul(&a->mantissa, &b->mantissa, &c->mantissa);
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
	struct bf zero;
	bigfloat_init(&zero);
	return(bigfloat_cmp(n, &zero));
}

void bf_shiftEXP(struct bf* n, int shift){
	if(shift == 0){
		return;
	}
	if(shift > 0){
		struct bn s_tmp;
		bignum_rshift(&n->mantissa.value, &s_tmp, shift);
		bignum_assign(&n->mantissa.value, &s_tmp);
		struct bn_s e_tmp;
		struct bn_s i_tmp;
		bignum_signed_from_int(&i_tmp, shift);
		bignum_signed_add(&n->exponent, &i_tmp, &e_tmp);
		bignum_signed_assign(&e_tmp, &n->exponent);
	}else{
		//shift < 0
		int bshift = -shift;
		struct bn s_tmp;
		bignum_lshift(&n->mantissa.value, &s_tmp, bshift);
		bignum_assign(&n->mantissa.value, &s_tmp);
		struct bn_s e_tmp;
		struct bn_s i_tmp;
		bignum_signed_from_int(&i_tmp, shift);
		bignum_signed_add(&n->exponent, &i_tmp, &e_tmp);
		bignum_signed_assign(&e_tmp, &n->exponent);
	}
}

void bigfloat_normalize(struct bf* n){
	//should leave it so that there is one place blank in mantissa, and adjust the exponent accordingly
	
	unsigned int currentBSR = bignum_bsr(&n->mantissa.value);

}

/*
 * TODO:
 * figure out how the normalize function should work
 * code it.
 * fix add.
 */
