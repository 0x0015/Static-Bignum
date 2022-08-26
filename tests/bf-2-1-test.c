#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../bf.h"

//from https://stackoverflow.com/questions/55766058/how-can-i-generate-random-doubles-in-c
double rd() {
	double o = ((double)rand() / (RAND_MAX));
	o *= 2;
	o -= 1;
	return(o);
}

int closeWithinEpsilon(double d1, double d2){
	return (fabs(d1-d2) < 0.1); //epsilon is 0.1 for now
}

int main(){
	printf("starting floating point stress test...\n");
	//srand(time(0));
	srand(0); //set seed for now
	
	bf a1;
	bf_2 a2;
	
	for(unsigned int i=0;i<10000;i++){
		double ua = rd() * 1000;
		bigfloat_from_double(&a1, ua);
		bigfloat_to_2(&a2, &a1);
		double aresult = bigfloat_to_double_2(&a2);
		if(!closeWithinEpsilon(ua, aresult)){
			printf("1->2: Failed to properly set a (should be %lf, got %lf)\n", ua, aresult);
		}
	}
	
	for(unsigned int i=0;i<10000;i++){
		double ua = rd() * 1000;
		bigfloat_from_double_2(&a2, ua);
		bigfloat_2_to_1(&a1, &a2);
		double aresult = bigfloat_to_double(&a1);
		if(!closeWithinEpsilon(ua, aresult)){
			printf("2->1: Failed to properly set a (should be %lf, got %lf)\n", ua, aresult);
		}
	}
	printf("2-1 conversions completed.\n");
	
}
