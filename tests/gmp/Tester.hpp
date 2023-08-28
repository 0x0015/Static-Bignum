#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <optional>
#include <gmpxx.h>

template<class T1, class T2> class Tester{
public:

	std::vector<std::string> inputValues;
	using t1OpFunc = std::function<T1(const T1&, const T1&)>;
	using t2OpFunc = std::function<T2(const T2&, const T2&)>;
	struct funcPair{
		t1OpFunc t1Func;
		t2OpFunc t2Func;
		std::string name;
	};
	std::vector<funcPair> funcPairs;

	unsigned int succeeded = 0;
	unsigned int failed = 0;

	bool checkPositive = false;

	void autoGenerateBasicOpFuncs(){
		funcPair plusFunc;
		plusFunc.t1Func = [](const T1& a, const T1& b){return a + b;};
		plusFunc.t2Func = [](const T2& a, const T2& b){return a + b;};
		plusFunc.name = "+";
		funcPairs.push_back(plusFunc);

		funcPair subFunc;
		subFunc.t1Func = [](const T1& a, const T1& b){return a - b;};
		subFunc.t2Func = [](const T2& a, const T2& b){return a - b;};
		subFunc.name = "-";
		funcPairs.push_back(subFunc);

		funcPair mulFunc;
		mulFunc.t1Func = [](const T1& a, const T1& b){return a * b;};
		mulFunc.t2Func = [](const T2& a, const T2& b){return a * b;};
		mulFunc.name = "*";
		funcPairs.push_back(mulFunc);

		funcPair divFunc;
		divFunc.t1Func = [](const T1& a, const T1& b){return a / b;};
		divFunc.t2Func = [](const T2& a, const T2& b){return a / b;};
		divFunc.name = "/";
		funcPairs.push_back(divFunc);
	}

	enum testResult{TEST_SUCCEEDED, TEST_FAILED, TEST_NOT_RAN};
	testResult performTest(const std::string& num1, const std::string& num2, const funcPair& testFunc, std::ostream& out){
		T1 gmp_a, gmp_b;
		if constexpr(std::is_same_v<T1, mpf_class>){
			gmp_a = T1(num1, 512, 16);
			gmp_b = T1(num2, 512, 16);
		}else{
			gmp_a = T1(num1, 16);
			gmp_b = T1(num2, 16);
		}
		T2 bn_a(num1);
		T2 bn_b(num2);

		T1 gmp_zero(0);

		if(testFunc.name == "/" && gmp_b == 0)
			return TEST_NOT_RAN;

		T1 gmp_tmp = testFunc.t1Func(gmp_a,gmp_b);
		T2 bn_tmp;

		bool failed = false;

		if(!checkPositive || gmp_tmp >= gmp_zero){
			bn_tmp = testFunc.t2Func(bn_a,bn_b);
			std::string gmp_res;
			if constexpr(std::is_same_v<T1, mpf_class>){
				mp_exp_t exp;
				gmp_res = gmp_tmp.get_str(exp, 16);
			}else{
				gmp_res = gmp_tmp.get_str(16);
			}
			std::string bn_res = bn_tmp.to_string();
			if(gmp_res != bn_res){
				out<<"Failed test! (num1 = "<<num1<<" "<<testFunc.name<<" num2 = "<<num2<<")"<<std::endl;
				out<<"\tgmp result: "<<gmp_res<<std::endl;
				out<<"\tbn result: "<<bn_res<<std::endl;
				failed = true;
			}
		}
		
		if(failed)
			return TEST_FAILED;
		return TEST_SUCCEEDED;
	}

	void test(std::ostream* output = nullptr){
		std::ostream* out_ptr = &std::cout;
		if(output != nullptr)
			out_ptr = output;
		std::ostream& out = *out_ptr;

		for(int i=0;i<inputValues.size();i++){
			for(int j=0;j<inputValues.size();j++){
				for(int k=0;k<funcPairs.size();k++){
					auto result = performTest(inputValues[i], inputValues[j], funcPairs[k], out);
					if(result == TEST_SUCCEEDED){
						succeeded++;
					}else if(result == TEST_FAILED){
						failed++;
					}
				}
			}
		}

		out<<"Finished tests with "<<succeeded<<" succeeding and "<<failed<<" failing."<<std::endl;
	}
};
