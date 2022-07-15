#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../bn.h"

uint32_t isqrt(uint32_t n){
	return(sqrt(n));//wow such casting
}

int main(){
	printf("starting unsigned intager stress test...\n");
	//srand(time(0));
	srand(0); //set seed for now
	bn a, b, c;
	for(unsigned int i=0;i<10000;i++){
		uint32_t ua = isqrt(rand()); 
		uint32_t ub = isqrt(rand()); 
		uint32_t uc = ua * ub;
		bignum_from_int(&a, ua);
		bignum_from_int(&b, ub);
		bignum_mul(&a, &b, &c);
		uint32_t aresult = bignum_to_int(&a);
		uint32_t bresult = bignum_to_int(&b);
		uint32_t cresult = bignum_to_int(&c);
		if(ua != aresult){
			printf("Failed to properly set a\n");
		}
		if(ub != bresult){
			printf("failed to properly set b\n");
		}
		if(cresult == uc){
			//printf("\tCorrect %u * %u = %u\n", ua, ub, uc);
		}else{
			printf("Incorrect %u * %u = %u (got %u)\n", ua, ub, uc, cresult);
		}
	}
	for(unsigned int i=0;i<10000;i++){
		uint32_t ua = rand(); 
		uint32_t ub = rand(); 
		uint32_t uc = ua + ub;
		bignum_from_int(&a, ua);
		bignum_from_int(&b, ub);
		bignum_add(&a, &b, &c);
		uint32_t aresult = bignum_to_int(&a);
		uint32_t bresult = bignum_to_int(&b);
		uint32_t cresult = bignum_to_int(&c);
		if(ua != aresult){
			printf("Failed to properly set a\n");
		}
		if(ub != bresult){
			printf("failed to properly set b\n");
		}
		if(cresult == uc){
			//printf("\tCorrect %u + %u = %u\n", ua, ub, uc);
		}else{
			printf("Incorrect %u + %u = %u (got %u)\n", ua, ub, uc, cresult);
		}
	}
	for(unsigned int i=0;i<10000;i++){
		uint32_t ua = rand(); 
		uint32_t ub = rand(); 
		uint32_t uc = ua / ub;
		bignum_from_int(&a, ua);
		bignum_from_int(&b, ub);
		bignum_div(&a, &b, &c);
		uint32_t aresult = bignum_to_int(&a);
		uint32_t bresult = bignum_to_int(&b);
		uint32_t cresult = bignum_to_int(&c);
		if(ua != aresult){
			printf("Failed to properly set a\n");
		}
		if(ub != bresult){
			printf("failed to properly set b\n");
		}
		if(cresult == uc){
			//printf("\tCorrect %u / %u = %u\n", ua, ub, uc);
		}else{
			printf("Incorrect %u / %u = %u (got %u)\n", ua, ub, uc, cresult);
		}
	}
	for(unsigned int i=0;i<10000;i++){
		uint32_t ua = rand(); 
		uint32_t ub = rand(); 
		uint32_t uc = ua - ub;
		bignum_from_int(&a, ua);
		bignum_from_int(&b, ub);
		bignum_sub(&a, &b, &c);
		uint32_t aresult = bignum_to_int(&a);
		uint32_t bresult = bignum_to_int(&b);
		uint32_t cresult = bignum_to_int(&c);
		if(ua != aresult){
			printf("Failed to properly set a\n");
		}
		if(ub != bresult){
			printf("failed to properly set b\n");
		}
		if(cresult == uc){
			//printf("\tCorrect %u - %u = %u\n", ua, ub, uc);
		}else{
			printf("Incorrect %u - %u = %u (got %u)\n", ua, ub, uc, cresult);
		}
	}
	printf("unsigned intager stress test done.\n");
}
