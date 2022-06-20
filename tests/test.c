#include <stdio.h>
#include "../bn.h"
#include "../bf.h"

void bigint_test(){
	printf("Bigint test:\n");
	struct bn num;
	struct bn result;
	struct bn tmp;
	char buf[1024];

	bignum_from_int(&num, 21);

	bignum_assign(&tmp, &num);
	bignum_dec(&tmp);
	bignum_mul(&tmp, &tmp, &result);

	int output = bignum_to_int(&result);
	bignum_to_string(&result, buf, sizeof(buf));
	printf("mul result: %s (%i)\n", buf, output);

	bignum_from_int(&num, 0);
	bignum_dec(&num);
	bignum_from_int(&tmp, 100);
	//bignum_div(&num, &tmp, &result);
	bignum_assign(&result, &num);

	output = bignum_to_int(&result);
	bignum_to_string(&result, buf, sizeof(buf));
	printf("div result: %s (%i)\n", buf, output);

	if(bignum_is_zero(&bn_zero)){
		printf("bn_zero is zero\n");
	}else{
		printf("bn_zero is NOT zero\n");
	}
}

void bigsigned_test(){
	printf("Bignum_signed test:\n");
	struct bn_s num;
	struct bn_s result;
	struct bn_s tmp;
	char buf[1024];


	bignum_signed_from_int(&num, 21);
	//bignum_from_int(&num.value, 21);

	bignum_signed_assign(&tmp, &num);
	bignum_signed_dec(&tmp);
	bignum_signed_mul(&tmp, &tmp, &result);

	int output = bignum_signed_to_int(&result);
	bignum_signed_to_string(&result, buf, sizeof(buf));
	printf("mul result: %s (%i)\n", buf, output);

	bignum_signed_from_int(&num, -1);
	bignum_signed_from_int(&tmp, 100);
	bignum_signed_add(&num, &tmp, &result);
	bignum_signed_assign(&num, &result);

	output = bignum_signed_to_int(&result);
	bignum_signed_to_string(&result, buf, sizeof(buf));
	printf("add result: %s (%i)\n", buf, output);


	bignum_signed_from_int(&num, -1);
	bignum_signed_from_int(&tmp, 100);
	bignum_signed_sub(&num, &tmp, &result);
	bignum_signed_assign(&num, &result);

	output = bignum_signed_to_int(&result);
	bignum_signed_to_string(&result, buf, sizeof(buf));
	printf("sub result: %s (%i)\n", buf, output);


	bignum_signed_from_int(&num, 20);
	bignum_signed_from_int(&tmp, 50);
	bignum_signed_sub(&num, &tmp, &result);
	bignum_signed_assign(&num, &result);

	output = bignum_signed_to_int(&result);
	bignum_signed_to_string(&result, buf, sizeof(buf));
	printf("sub (20-50) result: %s (%i)\n", buf, output);
}

void print_bf(struct bf* n, const char* name){
	char buf1[1024];
	char buf2[1024];
	int64_t mi = bignum_signed_to_int(&n->mantissa);
	int64_t ei = bignum_signed_to_int(&n->exponent);
	bignum_signed_to_string(&n->mantissa, buf1, sizeof(buf1));
	bignum_signed_to_string(&n->exponent, buf2, sizeof(buf2));

	printf("%s: result: mantissa: %s (%li), exponent: %s (%li)\n", name, buf1, mi, buf2, ei);
}

void bigfloat_test(){
	printf("Bigfloat test:\n");
	struct bf a;
	struct bf b;
	struct bf result1;
	struct bf result2;

	bigfloat_from_int(&a, 100);
	print_bf(&a, "original a");
	bigfloat_from_int(&b, 2000);
	print_bf(&b, "original b");
	//bigfloat_from_double(&b, 0.5);
	//bigfloat_from_int(&c, 2);
	bigfloat_mul(&a, &b, &result1);
	bigfloat_add(&a, &b, &result2);
	//bigfloat_assign(&a, &result);
	//bigfloat_mul(&a, &c, &result);
	//bigfloat_assign(&result, &a);
	print_bf(&a, "a");
	print_bf(&b, "b");
	print_bf(&result1, "result 1");
	print_bf(&result2, "result 2");
}

int main(){
	bigint_test();
	bigsigned_test();
	bigfloat_test();
	return 0;
}
