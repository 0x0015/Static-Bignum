#include <stdio.h>
#include "bn.h"
#include "bf.h"

void bigint_test(){
	printf("Bigint test:\n");
	struct bn num;
	struct bn result;
	struct bn tmp;
	char buf[1024];

	bignum_from_int(&num, 123456);

	bignum_assign(&tmp, &num);
	bignum_dec(&tmp);
	bignum_mul(&tmp, &tmp, &result);

	int output = bignum_to_int(&result);
	bignum_to_string(&result, buf, sizeof(buf));
	printf("mul result: %s (%i)\n", buf, output);

	bignum_from_int(&num, -1);
	bignum_from_int(&tmp, 100);
	//bignum_div(&num, &tmp, &result);
	bignum_assign(&num, &result);

	output = bignum_to_int(&result);
	bignum_to_string(&result, buf, sizeof(buf));
	printf("div result: %s (%i)\n", buf, output);

	printf("num bsr: %u\n", bigint_bsr(&num));
}

void bigfloat_test(){
	printf("Bigint test:\n");
	struct bf a;
	struct bf b;
	struct bf result;

	bigfloat_from_int(&a, 100);
	bigfloat_from_double(&b, 3.14159);
	bigfloat_mul(&a, &b, &result);

	char buf1[1024];
	char buf2[1024];
	bignum_to_string(&result.mantissa, buf1, sizeof(buf1));
	bignum_to_string(&result.exponent, buf2, sizeof(buf2));

	printf("result: mantissa: %s, exponent: %s\n", buf1, buf2);
}

int main(){
	bigint_test();
	bigfloat_test();
	return 0;
}
