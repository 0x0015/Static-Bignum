#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../bf.h"

//from https://stackoverflow.com/questions/55766058/how-can-i-generate-random-doubles-in-c
double rd() {
    uint64_t r53 = ((uint64_t)(rand()) << 21) ^ (rand() >> 2);
    return (double)r53 / 9007199254740991.0; // 2^53 - 1
}

int closeWithinEpsilon(double d1, double d2){
	return (fabs(d1-d2) < 0.1); //epsilon is 0.1 for now
}

void checkConversions(){
	bf test;
	double currentTest = 3.14159;
	bigfloat_from_double(&test, currentTest);
	double toutput = bigfloat_to_double(&test);
	printf("expected: %lf, got %lf\n", currentTest, toutput);
	currentTest = 1234.567;
	bigfloat_from_double(&test, currentTest);
	toutput = bigfloat_to_double(&test);
	printf("expected: %lf, got %lf\n", currentTest, toutput);
	currentTest = 54.543654;
	bigfloat_from_double(&test, currentTest);
	toutput = bigfloat_to_double(&test);
	printf("expected: %lf, got %lf\n", currentTest, toutput);
	currentTest = 0.54543654;
	bigfloat_from_double(&test, currentTest);
	toutput = bigfloat_to_double(&test);
	printf("expected: %lf, got %lf\n", currentTest, toutput);
	currentTest = 0.054543654;
	bigfloat_from_double(&test, currentTest);
	toutput = bigfloat_to_double(&test);
	printf("expected: %lf, got %lf\n", currentTest, toutput);
	currentTest = 0.001;
	bigfloat_from_double(&test, currentTest);
	toutput = bigfloat_to_double(&test);
	printf("expected: %lf, got %lf\n", currentTest, toutput);
	currentTest = 100;
	bigfloat_from_double(&test, currentTest);
	toutput = bigfloat_to_double(&test);
	printf("expected: %lf, got %lf\n", currentTest, toutput);
	unsigned int currentTest_i = 42;
	bigfloat_from_int(&test, currentTest_i);
	toutput = bigfloat_to_double(&test);
	printf("expected: %i, got %lf\n", currentTest_i, toutput);
	currentTest_i = 4200;
	bigfloat_from_int(&test, currentTest_i);
	toutput = bigfloat_to_double(&test);
	printf("expected: %i, got %lf\n", currentTest_i, toutput);
}

int main(){
	printf("starting floating point stress test...\n");
	//srand(time(0));
	srand(0); //set seed for now
	
	//checkConversions();
	//this seems to be working, so onto the actual test
	
	bf a, b, c;
	for(unsigned int i=0;i<10;i++){
		double ua = rd() * 1000; 
		double ub = rd() * 1000; 
		double uc = ua * ub;
		bigfloat_from_double(&a, ua);
		bigfloat_from_double(&b, ub);
		bigfloat_mul(&a, &b, &c);
		double aresult = bigfloat_to_double(&a);
		double bresult = bigfloat_to_double(&b);
		double cresult = bigfloat_to_double(&c);
		if(!closeWithinEpsilon(ua, aresult)){
			printf("Failed to properly set a\n");
		}
		if(!closeWithinEpsilon(ub, bresult)){
			printf("failed to properly set b\n");
		}
		if(closeWithinEpsilon(cresult, uc)){
			//printf("\tCorrect %lf * %lf = %lf (got %lf)\n", ua, ub, uc, cresult);
		}else{
			printf("Incorrect %lf * %lf = %lf (got %lf, a=%lf, b=%lf)\n", ua, ub, uc, cresult, aresult, bresult);
		}
	}
	for(unsigned int i=0;i<10;i++){
		double ua = rd() * 1000; 
		double ub = rd() * 1000; 
		double uc = ua / ub;
		bigfloat_from_double(&a, ua);
		bigfloat_from_double(&b, ub);
		bigfloat_div(&a, &b, &c);
		double aresult = bigfloat_to_double(&a);
		double bresult = bigfloat_to_double(&b);
		double cresult = bigfloat_to_double(&c);
		if(!closeWithinEpsilon(ua, aresult)){
			printf("Failed to properly set a\n");
		}
		if(!closeWithinEpsilon(ub, bresult)){
			printf("failed to properly set b\n");
		}
		if(closeWithinEpsilon(cresult, uc)){
			//printf("\tCorrect %lf / %lf = %lf (got %lf)\n", ua, ub, uc, cresult);
		}else{
			printf("Incorrect %lf / %lf = %lf (got %lf, a=%lf, b=%lf)\n", ua, ub, uc, cresult, aresult, bresult);
		}
	}
	srand(0);
	for(unsigned int i=0;i<10;i++){
		double ua = rd() * 1000; 
		double ub = rd() * 1000; 
		double uc = ua + ub;
		bigfloat_from_double(&a, ua);
		bigfloat_from_double(&b, ub);
		bigfloat_add(&a, &b, &c);
		double aresult = bigfloat_to_double(&a);
		double bresult = bigfloat_to_double(&b);
		double cresult = bigfloat_to_double(&c);
		if(!closeWithinEpsilon(ua, aresult)){
			printf("Failed to properly set a\n");
		}
		if(!closeWithinEpsilon(ub, bresult)){
			printf("failed to properly set b\n");
		}
		if(closeWithinEpsilon(cresult, uc)){
			//printf("\tCorrect %lf + %lf = %lf (got %lf)\n", ua, ub, uc, cresult);
		}else{
			printf("Incorrect %lf + %lf = %lf (got %lf, a=%lf, b=%lf)\n", ua, ub, uc, cresult, aresult, bresult);
		}
	}
	printf("floating point stress test done.\n");
	
}
