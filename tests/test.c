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

	bignum_from_int(&num, -1);
	bignum_from_int(&tmp, 100);
	//bignum_div(&num, &tmp, &result);
	bignum_assign(&num, &result);

	output = bignum_to_int(&result);
	bignum_to_string(&result, buf, sizeof(buf));
	printf("div result: %s (%i)\n", buf, output);
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

void print_bf(struct bf* n){
	char buf1[1024];
	char buf2[1024];
	uint64_t mi = bignum_to_int(&n->mantissa.value);
	uint64_t ei = bignum_to_int(&n->exponent.value);
	bignum_to_string(&n->mantissa.value, buf1, sizeof(buf1));
	bignum_to_string(&n->exponent.value, buf2, sizeof(buf2));

	printf("result: mantissa: %s (%lu), exponent: %s (%lu)\n", buf1, mi, buf2, ei);
}

void bigfloat_test(){
	printf("Bigint test:\n");
	struct bf a;
	struct bf b;
	struct bf c;
	struct bf result;

	bigfloat_from_int(&a, 100);
	print_bf(&a);
	//bigfloat_from_int(&b, 2000);
	bigfloat_from_double(&b, 0.5);
	bigfloat_from_double(&c, 2);
	bigfloat_mul(&a, &b, &result);
	bigfloat_assign(&a, &result);
	bigfloat_mul(&a, &c, &result);
	//bigfloat_assign(&result, &a);
	print_bf(&a);
	print_bf(&b);
	print_bf(&c);
	print_bf(&result);
}

int main(){
	bigint_test();
	bigsigned_test();
	bigfloat_test();
	return 0;
}
