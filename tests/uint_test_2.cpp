#include <iostream>
#include "Tester.hpp"
#include <cmath>

#include "../bn.h"

uint32_t isqrt(uint32_t n){
	return(sqrt(n));//wow such casting
}

int main(){
	typedef bn_2 T1;
	typedef uint32_t T2;

	Tester<T1, T2> test;
	test.saveFailure = true;
	test.preDeterminedRHS = false;//division by zero prevention
	test.preDetermined = {0};

	test.T1ToT2 = [](T2& t2, T1& t1){
		t2 = bignum_to_int_2(&t1);
	};
	test.T2ToT1 = [](T1& t1, T2& t2){
		bignum_from_int_2(&t1, t2);
	};
	test.T1assign = [](T1& n1, T1& n2){
		bignum_assign_2(&n1, &n2);
	};
	test.T2assign = [](T2& n1, T2& n2){
		n1 = n2;
	};
	test.compare = [](T2& n1, T2& n2) -> bool{
		return(n1 == n2);
	};
	test.getRandomT2 = [](T2& n){
		n = isqrt(rand());
	};
	test.printT2 = [](T2& n){
		std::cout<<n;
	};
	test.printT1 = [](T1& n){
		T2 in;
		in = bignum_to_int_2(&n);
		std::cout<<in;
	};

	test.testFuncs.push_back(Tester_Func<T1, T2>{"+", "unsigned addition", [](T1& a, T1& b, T1& c){bignum_add_2(&a, &b, &c);}, [](T2& a, T2& b, T2& c){c = a + b;}});

	//test.testFuncs.push_back(Tester_Func<T1, T2>{"-", "unsigned subtraction", [](T1& a, T1& b, T1& c){bignum_sub(&a, &b, &c);}, [](T2& a, T2& b, T2& c){c = a - b;}});
	//overflow will occur

	test.testFuncs.push_back(Tester_Func<T1, T2>{"*", "unsigned multiplication", [](T1& a, T1& b, T1& c){bignum_mul_2(&a, &b, &c);}, [](T2& a, T2& b, T2& c){c = a * b;}});

	test.testFuncs.push_back(Tester_Func<T1, T2>{"/", "unsigned division", [](T1& a, T1& b, T1& c){bignum_div_2(&a, &b, &c);}, [](T2& a, T2& b, T2& c){c = a / b;}});

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

