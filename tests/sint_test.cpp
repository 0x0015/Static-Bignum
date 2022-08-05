#include <iostream>
#include "Tester.hpp"
#include <cmath>

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
	Tester<bn_s, int32_t> test;
	test.saveFailure = true;
	test.preDeterminedRHS = false;//division by zero prevention
	test.preDetermined = {0};

	test.T1ToT2 = [](int32_t& t2, bn_s& t1){
		t2 = bignum_signed_to_int(&t1);
	};
	test.T2ToT1 = [](bn_s& t1, int32_t& t2){
		bignum_signed_from_int(&t1, t2);
	};
	test.T1assign = [](bn_s& n1, bn_s& n2){
		bignum_signed_assign(&n1, &n2);
	};
	test.T2assign = [](int32_t& n1, int32_t& n2){
		n1 = n2;
	};
	test.compare = [](int32_t& n1, int32_t& n2) -> bool{
		return(n1 == n2);
	};
	test.getRandomT2 = [](int32_t& n){
		n = isqrt((RAND_MAX/2)-rand());
	};
	test.printT2 = [](int32_t& n){
		std::cout<<n;
	};
	test.printT1 = [](bn_s& n){
		int32_t in;
		in = bignum_signed_to_int(&n);
		std::cout<<in;
	};

	test.testFuncs.push_back(Tester_Func<bn_s, int32_t>{"+", "unsigned addition", [](bn_s& a, bn_s& b, bn_s& c){bignum_signed_add(&a, &b, &c);}, [](int32_t& a, int32_t& b, int32_t& c){c = a + b;}});

	test.testFuncs.push_back(Tester_Func<bn_s, int32_t>{"-", "unsigned subtraction", [](bn_s& a, bn_s& b, bn_s& c){bignum_signed_sub(&a, &b, &c);}, [](int32_t& a, int32_t& b, int32_t& c){c = a - b;}});
	//overflow will occur

	test.testFuncs.push_back(Tester_Func<bn_s, int32_t>{"*", "unsigned multiplication", [](bn_s& a, bn_s& b, bn_s& c){bignum_signed_mul(&a, &b, &c);}, [](int32_t& a, int32_t& b, int32_t& c){c = a * b;}});

	test.testFuncs.push_back(Tester_Func<bn_s, int32_t>{"/", "unsigned division", [](bn_s& a, bn_s& b, bn_s& c){bignum_signed_div(&a, &b, &c);}, [](int32_t& a, int32_t& b, int32_t& c){c = a / b;}});

	test.test();
	std::cout<<"Completed tests!  num succeeded: "<<test.numSucceeded<<", num failed: "<<test.numFailed<<std::endl;
	std::cout<<"outputs:"<<std::endl;
	for(auto& o : test.outputs){
		std::cout<<o.testName<<": ";
		test.printT1(o.T1input.first);
		std::cout<<"(";
		test.printT2(o.T2input.first);
		std::cout<<") "<<o.funcName<<" ";
		test.printT1(o.T1input.second);
		std::cout<<"(";
		test.printT2(o.T2input.second);
		std::cout<<") -> ";
		test.printT1(o.T1output);
		std::cout<<"(";
		test.printT2(o.T2output);
		std::cout<<")"<<std::endl;
	}
}

