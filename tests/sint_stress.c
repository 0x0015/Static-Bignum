#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../bn_s.h"

int32_t isqrt(int32_t in){
	int32_t n = in;
	if(n < 0)
		n = -n;
	int32_t o = sqrt(n);//wow such casting
	if(in < 0)
		o = -o;
	return o;
}

int main(){
	printf("starting signed intager stress test...\n");
	//srand(time(0));
	srand(0); //set seed for now
	bn_s a, b, c;
	for(unsigned int i=0;i<10000;i++){
		int32_t ua = isqrt((RAND_MAX/2)-rand()); 
		int32_t ub = isqrt((RAND_MAX/2)-rand()); 
		int32_t uc = ua * ub;
		bignum_signed_from_int(&a, ua);
		bignum_signed_from_int(&b, ub);
		bignum_signed_mul(&a, &b, &c);
		int32_t aresult = bignum_signed_to_int(&a);
		int32_t bresult = bignum_signed_to_int(&b);
		int32_t cresult = bignum_signed_to_int(&c);
		if(ua != aresult){
			printf("Failed to properly set a\n");
		}
		if(ub != bresult){
			printf("failed to properly set b\n");
		}
		if(cresult == uc){
			//printf("\tCorrect %i * %i = %i\n", ua, ub, uc);
		}else{
			printf("Incorrect %i * %i = %i (got %i)\n", ua, ub, uc, cresult);
		}
	}
	for(unsigned int i=0;i<10000;i++){
		int32_t ua = (RAND_MAX/2)-rand(); 
		int32_t ub = (RAND_MAX/2)-rand(); 
		int32_t uc = ua + ub;
		bignum_signed_from_int(&a, ua);
		bignum_signed_from_int(&b, ub);
		bignum_signed_add(&a, &b, &c);
		int32_t aresult = bignum_signed_to_int(&a);
		int32_t bresult = bignum_signed_to_int(&b);
		int32_t cresult = bignum_signed_to_int(&c);
		if(ua != aresult){
			printf("Failed to properly set a\n");
		}
		if(ub != bresult){
			printf("failed to properly set b\n");
		}
		if(cresult == uc){
			//printf("\tCorrect %i + %i = %i\n", ua, ub, uc);
		}else{
			printf("Incorrect %i + %i = %i (got %i)\n", ua, ub, uc, cresult);
		}
	}
	for(unsigned int i=0;i<10000;i++){
		int32_t ua = (RAND_MAX/2)-rand(); 
		int32_t ub = (RAND_MAX/2)-rand(); 
		int32_t uc = ua / ub;
		bignum_signed_from_int(&a, ua);
		bignum_signed_from_int(&b, ub);
		bignum_signed_div(&a, &b, &c);
		int32_t aresult = bignum_signed_to_int(&a);
		int32_t bresult = bignum_signed_to_int(&b);
		int32_t cresult = bignum_signed_to_int(&c);
		if(ua != aresult){
			printf("Failed to properly set a\n");
		}
		if(ub != bresult){
			printf("failed to properly set b\n");
		}
		if(cresult == uc){
			//printf("\tCorrect %i / %i = %i\n", ua, ub, uc);
		}else{
			printf("Incorrect %i / %i = %i (got %i)\n", ua, ub, uc, cresult);
		}
	}
	for(unsigned int i=0;i<10000;i++){
		int32_t ua = (RAND_MAX/2)-rand(); 
		int32_t ub = (RAND_MAX/2)-rand(); 
		int32_t uc = ua - ub;
		bignum_signed_from_int(&a, ua);
		bignum_signed_from_int(&b, ub);
		bignum_signed_sub(&a, &b, &c);
		int32_t aresult = bignum_signed_to_int(&a);
		int32_t bresult = bignum_signed_to_int(&b);
		int32_t cresult = bignum_signed_to_int(&c);
		if(ua != aresult){
			printf("Failed to properly set a\n");
		}
		if(ub != bresult){
			printf("failed to properly set b\n");
		}
		if(cresult == uc){
			//printf("\tCorrect %i - %i = %i\n", ua, ub, uc);
		}else{
			printf("Incorrect %i - %i = %i (got %i)\n", ua, ub, uc, cresult);
		}
	}
	printf("signed intager stress test done.\n");
}
