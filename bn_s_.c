

void PPCAT(bignum_signed_init, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* n){
	PPCAT(bignum_init, BN_PREFIX)(&n->value);
	n->sign = 0;
}

void PPCAT(bignum_signed_from_bn, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* n, BN_VAR_PREFIX struct PPCAT(bn, BN_PREFIX)* bigint){
	PPCAT(bignum_assign, BN_PREFIX)(&n->value, bigint);
	n->sign = 0;
}

void PPCAT(bignum_signed_from_int, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* n, int64_t value){
	int64_t value_abs = value;
	uint8_t sign = 0;
	if(value_abs < 0){
		value_abs = -value_abs;
		sign = 1;
	}
	PPCAT(bignum_from_int, BN_PREFIX)(&n->value, (uint64_t)value_abs);
	n->sign = sign;
}

int32_t PPCAT(bignum_signed_to_int, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* n){
	uint32_t u_output = PPCAT(bignum_to_int, BN_PREFIX)(&n->value);
	int64_t output = u_output;
	if(n->sign != 0){
		output = -output;
	} 
	return(output);
}

void PPCAT(bignum_signed_add, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* c){
	if(a->sign == b->sign){
		PPCAT(bignum_add, BN_PREFIX)(&a->value, &b->value, &c->value);
		c->sign = a->sign;
	}else if(!a->sign && b->sign){
		int result = PPCAT(bignum_cmp, BN_PREFIX)(&a->value, &b->value);
		switch(result){
			case LARGER:{
					    PPCAT(bignum_sub, BN_PREFIX)(&a->value, &b->value, &c->value);
					    c->sign = 0;
				    }
				    break;
			case SMALLER:{
					    PPCAT(bignum_sub, BN_PREFIX)(&b->value, &a->value, &c->value);
					    c->sign = 1;
				     }
				     break;
			case EQUAL:{
					   PPCAT(bignum_init, BN_PREFIX)(&c->value);
					   c->sign = 0;
				   }
				   break;
		}
	}else if(a->sign && !b->sign){
		int result = PPCAT(bignum_cmp, BN_PREFIX)(&b->value, &a->value);
		switch(result){
			case LARGER:{
					    PPCAT(bignum_sub, BN_PREFIX)(&b->value, &a->value, &c->value);
					    c->sign = 0;
				    }
				    break;
			case SMALLER:{
					    PPCAT(bignum_sub, BN_PREFIX)(&a->value, &b->value, &c->value);
					    c->sign = 1;
				     }
				     break;
			case EQUAL:{
					   PPCAT(bignum_init, BN_PREFIX)(&c->value);
					   c->sign = 0;
				   }
				   break;
		}
	}	
}

void PPCAT(bignum_signed_sub, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* c){
	struct PPCAT(bn_s, BN_PREFIX) tmp;
	PPCAT(bignum_assign, BN_PREFIX)(&tmp.value, &b->value);
	if(b->sign == 0){
		tmp.sign = 1;
	}else{
		tmp.sign = 0;
	}
	PPCAT(bignum_signed_add, BN_PREFIX)(a, &tmp, c);
}


void PPCAT(bignum_signed_mul, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* c){
	if(a->sign == b->sign){
		c->sign = 0;
		PPCAT(bignum_mul, BN_PREFIX)(&a->value, &b->value, &c->value);
	}else{
		c->sign = 1;
		PPCAT(bignum_mul, BN_PREFIX)(&a->value, &b->value, &c->value);
	}
}


void PPCAT(bignum_signed_div, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* b, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* c){
	if(a->sign == b->sign){
		c->sign = 0;
		PPCAT(bignum_div, BN_PREFIX)(&a->value, &b->value, &c->value);
	}else{
		c->sign = 1;
		PPCAT(bignum_div, BN_PREFIX)(&a->value, &b->value, &c->value);
	}
}

int PPCAT(bignum_signed_cmp, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* a, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* b){
	if(!a->sign && !b->sign){
		//both are positive
		return(PPCAT(bignum_cmp, BN_PREFIX)(&a->value, &b->value));
	}else if(!a->sign && b->sign){
		return(LARGER);
	}else if(a->sign && !b->sign){
		return(SMALLER);
	}else{//both are negative
		int result = PPCAT(bignum_cmp, BN_PREFIX)(&a->value, &b->value);
		switch(result){
			case LARGER:
				return(SMALLER);
			case SMALLER:
				return(LARGER);
			default:
				return(result);
		}
	}
}

void PPCAT(bignum_signed_assign, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* dst, BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* src){
	PPCAT(bignum_assign, BN_PREFIX)(&dst->value, &src->value);
	dst->sign = src->sign;
}

int PPCAT(bignum_signed_is_zero, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* n){
	return(PPCAT(bignum_is_zero, BN_PREFIX)(&n->value));
}

int PPCAT(bignum_signed_is_negative, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* n){
	return(n->sign == 1);
}

void PPCAT(bignum_signed_inc, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* n){
	if(PPCAT(bignum_signed_is_zero, BN_PREFIX)(n)){
		PPCAT(bignum_inc, BN_PREFIX)(&n->value);
		n->sign = 1;
		return;
	}else{
		if(n->sign != 0){
			PPCAT(bignum_dec, BN_PREFIX)(&n->value);
		}else{
			PPCAT(bignum_inc, BN_PREFIX)(&n->value);
		}
	}
}


void PPCAT(bignum_signed_dec, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* n){
	if(PPCAT(bignum_signed_is_zero, BN_PREFIX)(n)){
		PPCAT(bignum_inc, BN_PREFIX)(&n->value);
		n->sign = 0;
		return;
	}else{
		if(n->sign != 0){
			PPCAT(bignum_inc, BN_PREFIX)(&n->value);
		}else{
			PPCAT(bignum_dec, BN_PREFIX)(&n->value);
		}
	}

}

#ifndef BN_NO_STDIO
void PPCAT(bignum_signed_to_string, BN_PREFIX)(BN_VAR_PREFIX struct PPCAT(bn_s, BN_PREFIX)* n, char* str, int maxsize){
	if(n->sign == 0){
		PPCAT(bignum_to_string, BN_PREFIX)(&n->value, str, maxsize);
		return;
	}
	//the number is negative
	if(maxsize < 1){
		return;
	}
	str[0] = '-';
	PPCAT(bignum_to_string, BN_PREFIX)(&n->value, str+1, maxsize-2);//must have an even number of bytes
}
#endif
