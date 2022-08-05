#include <iostream>
#include "Tester.hpp"
#include <cmath>

#include "../bn.h"

uint32_t isqrt(uint32_t n){
	return(sqrt(n));//wow such casting
}

int main(){
	Tester<bn, uint32_t> test;
	test.saveFailure = true;
	test.preDeterminedRHS = false;//division by zero prevention
	test.preDetermined = {0};

	test.T1ToT2 = [](uint32_t& t2, bn& t1){
		t2 = bignum_to_int(&t1);
	};
	test.T2ToT1 = [](bn& t1, uint32_t& t2){
		bignum_from_int(&t1, t2);
	};
	test.T1assign = [](bn& n1, bn& n2){
		bignum_assign(&n1, &n2);
	};
	test.T2assign = [](uint32_t& n1, uint32_t& n2){
		n1 = n2;
	};
	test.compare = [](uint32_t& n1, uint32_t& n2) -> bool{
		return(n1 == n2);
	};
	test.getRandomT2 = [](uint32_t& n){
		n = isqrt(rand());
	};
	test.printT2 = [](uint32_t& n){
		std::cout<<n;
	};
	test.printT1 = [](bn& n){
		uint32_t in;
		in = bignum_to_int(&n);
		std::cout<<in;
	};

	test.testFuncs.push_back(Tester_Func<bn, uint32_t>{"+", "unsigned addition", [](bn& a, bn& b, bn& c){bignum_add(&a, &b, &c);}, [](uint32_t& a, uint32_t& b, uint32_t& c){c = a + b;}});

	//test.testFuncs.push_back(Tester_Func<bn, uint32_t>{"-", "unsigned subtraction", [](bn& a, bn& b, bn& c){bignum_sub(&a, &b, &c);}, [](uint32_t& a, uint32_t& b, uint32_t& c){c = a - b;}});
	//overflow will occur

	test.testFuncs.push_back(Tester_Func<bn, uint32_t>{"*", "unsigned multiplication", [](bn& a, bn& b, bn& c){bignum_mul(&a, &b, &c);}, [](uint32_t& a, uint32_t& b, uint32_t& c){c = a * b;}});

	test.testFuncs.push_back(Tester_Func<bn, uint32_t>{"/", "unsigned division", [](bn& a, bn& b, bn& c){bignum_div(&a, &b, &c);}, [](uint32_t& a, uint32_t& b, uint32_t& c){c = a / b;}});

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

