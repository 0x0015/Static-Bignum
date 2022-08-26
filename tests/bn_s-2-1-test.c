#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../bn_s.h"


int main(){
	printf("starting floating point stress test...\n");
	//srand(time(0));
	srand(0); //set seed for now
	
	bn_s a1;
	bn_s_2 a2;
	for(unsigned int i=0;i<10000;i++){
		int32_t ua = (RAND_MAX/2)-rand();
		bignum_signed_from_int(&a1, ua);
		bignum_signed_to_2(&a2, &a1);
		int32_t aresult = bignum_signed_to_int_2(&a2);
		if(ua != aresult){
			printf("1->2: Failed to properly set a (should be %i, got %i)\n", ua, aresult);
		}
	}
	for(unsigned int i=0;i<10000;i++){
		int32_t ua = (RAND_MAX/2)-rand();
		bignum_signed_from_int_2(&a2, ua);
		bignum_signed_2_to_1(&a1, &a2);
		int32_t aresult = bignum_signed_to_int(&a1);
		if(ua != aresult){
			printf("2->1: Failed to properly set a (should be %i, got %i)\n", ua, aresult);
		}
	}
	printf("2-1 conversions completed.\n");
	
}
