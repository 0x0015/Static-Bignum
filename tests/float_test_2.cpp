#include <iostream>
#include "Tester.hpp"
#include <cmath>

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
	Tester<bf_2, double> test;
	test.saveFailure = true;
	test.preDeterminedRHS = false;//division by zero prevention
	test.preDetermined = {0, 0.1, 0.01, 0.001, 0.0001, 0.00001, -0.1, -0.01, -0.001, -0.0001, -0.00001};


	test.T1ToT2 = [](double& t2, bf_2& t1){
		t2 = bigfloat_to_double_2(&t1);
	};
	test.T2ToT1 = [](bf_2& t1, double& t2){
		bigfloat_from_double_2(&t1, t2);
	};
	test.T1assign = [](bf_2& n1, bf_2& n2){
		bigfloat_assign_2(&n1, &n2);
	};
	test.T2assign = [](double& n1, double& n2){
		n1 = n2;
	};
	test.compare = [](double& n1, double& n2) -> bool{
		return(closeWithinEpsilon(n1, n2));
	};
	test.getRandomT2 = [](double& n){
		n = rd();
	};
	test.printT2 = [](double& n){
		std::cout<<n;
	};
	test.printT1 = [](bf_2& n){
		double in;
		in = bigfloat_to_double_2(&n);
		std::cout<<in;
	};

	test.testFuncs.push_back(Tester_Func<bf_2, double>{"+", "unsigned addition", [](bf_2& a, bf_2& b, bf_2& c){bigfloat_add_2(&a, &b, &c);}, [](double& a, double& b, double& c){c = a + b;}});

	test.testFuncs.push_back(Tester_Func<bf_2, double>{"-", "unsigned subtraction", [](bf_2& a, bf_2& b, bf_2& c){bigfloat_sub_2(&a, &b, &c);}, [](double& a, double& b, double& c){c = a - b;}});
	//overflow will occur

	test.testFuncs.push_back(Tester_Func<bf_2, double>{"*", "unsigned multiplication", [](bf_2& a, bf_2& b, bf_2& c){bigfloat_mul_2(&a, &b, &c);}, [](double& a, double& b, double& c){c = a * b;}});

	test.testFuncs.push_back(Tester_Func<bf_2, double>{"/", "unsigned division", [](bf_2& a, bf_2& b, bf_2& c){bigfloat_div_2(&a, &b, &c);}, [](double& a, double& b, double& c){c = a / b;}});

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

