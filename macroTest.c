#include "bn.h"
#include "bn_s.h"

int main(){
	bn test1;
	bn_2 test2;
	bignum_from_int(&test1, 5);
	bignum_from_int_2(&test2, 10);
	bn_s test1_s;
	bn_s_2 test2_s;
	bignum_signed_from_int(&test1_s, -5);
	bignum_signed_from_int_2(&test2_s, -10);
}

//gcc macroTest.c bn.c bn_s.c -Wall -o macroTest
//still working on bf
