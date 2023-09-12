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
	using standaloneFunc = std::function<bool(const std::string&)>;
	using t1BoolFunc = std::function<bool(const T1&, const T1&)>;
	using t2BoolFunc = std::function<bool(const T2&, const T2&)>;
	struct funcPair{
		t1OpFunc t1Func;
		t2OpFunc t2Func;
		std::string name;
	};
	struct funcBoolPair{
		t1BoolFunc t1Func;
		t2BoolFunc t2Func;
		std::string name;
	};
	std::vector<funcPair> funcPairs;
	std::vector<std::pair<standaloneFunc, std::string>> standaloneFuncs;
	std::vector<funcBoolPair> funcBoolPairs;

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

	void autoGenerateStrTest(){
		standaloneFuncs.push_back({[](const std::string& str){
			return T2(str).to_string() == str;
			}, "to_string"});
	}

	void autoGenerateBasicComparisonFuncs(){
		funcBoolPair gFunc;
		gFunc.t1Func = [](const T1& a, const T1& b){return a > b;};
		gFunc.t2Func = [](const T2& a, const T2& b){return a > b;};
		gFunc.name = ">";
		funcBoolPairs.push_back(gFunc);

		funcBoolPair geFunc;
		geFunc.t1Func = [](const T1& a, const T1& b){return a >= b;};
		geFunc.t2Func = [](const T2& a, const T2& b){return a >= b;};
		geFunc.name = ">=";
		funcBoolPairs.push_back(geFunc);

		funcBoolPair lFunc;
		lFunc.t1Func = [](const T1& a, const T1& b){return a < b;};
		lFunc.t2Func = [](const T2& a, const T2& b){return a < b;};
		lFunc.name = "<";
		funcBoolPairs.push_back(lFunc);

		funcBoolPair leFunc;
		leFunc.t1Func = [](const T1& a, const T1& b){return a <= b;};
		leFunc.t2Func = [](const T2& a, const T2& b){return a <= b;};
		leFunc.name = "<=";
		funcBoolPairs.push_back(leFunc);
	}

	bool smartStringComp(const std::string& str1, const std::string& str2){
		auto minLen = std::min(str1.length(), str2.length()) - 1;//if one rounds, the other will have a different last digit
		return str1.substr(0, minLen) == str2.substr(0, minLen);
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
				if((int)exp > 0 && gmp_res != ""){
					for(int i=0;i<exp;i++)
						gmp_res += ' ';
					if(gmp_res[0] == '-')
						exp++;
					gmp_res = gmp_res.substr(0, exp) + "." + gmp_res.substr(exp, gmp_res.length() - exp);
					
					gmp_res.erase(std::remove(gmp_res.begin(), gmp_res.end(), ' '), gmp_res.end());
					gmp_res.erase ( gmp_res.find_last_not_of('0') + 1, std::string::npos );
					gmp_res.erase ( gmp_res.find_last_not_of('.') + 1, std::string::npos );
				}
				if((int)exp < 0 && gmp_res != ""){
					std::string padStr = "";
					for(int i=0;i<std::abs((int)exp);i++){
						padStr += '0';
					}
					if(gmp_res[0] != '-')
						gmp_res = "." + padStr + gmp_res;
					else gmp_res = "-." + padStr + gmp_res.substr(1, gmp_res.length() - 2);
				}
				if(exp == 0 && gmp_res != ""){
					if(gmp_res[0] != '-')
						gmp_res = "." + gmp_res;
					else gmp_res = "-." + gmp_res.substr(1, gmp_res.length() - 2);
				}
				if(gmp_res == "")
					gmp_res = "0";
			}else{
				gmp_res = gmp_tmp.get_str(16);
			}
			
			//gmp eliminates ALL trailing zeros, even if that's the whole number
			if(gmp_res == "")
				gmp_res = "0";

			std::string bn_res = bn_tmp.to_string();

			bool compCheckedOut = false;
			if constexpr(std::is_same_v<T1, mpf_class>)
				compCheckedOut = !smartStringComp(gmp_res,bn_res) && gmp_res.length() > 0 && bn_res.length() > 0;
			else
				compCheckedOut = gmp_res != bn_res;
			if(compCheckedOut){
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

	testResult performCompTest(const std::string& num1, const std::string& num2, const funcBoolPair& testFunc, std::ostream& out){
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

		bool gmp_tmp = testFunc.t1Func(gmp_a,gmp_b);
		bool bn_tmp;

		bool failed = false;

			bn_tmp = testFunc.t2Func(bn_a,bn_b);

			if(gmp_tmp != bn_tmp){
				out<<"Failed test! (num1 = "<<num1<<" "<<testFunc.name<<" num2 = "<<num2<<")"<<std::endl;
				out<<"\tgmp result: "<<(int)gmp_tmp<<std::endl;
				out<<"\tbn result: "<<(int)bn_tmp<<std::endl;
				failed = true;
			}
		
		
		if(failed)
			return TEST_FAILED;
		return TEST_SUCCEEDED;
	}

	testResult performTest2(const std::string& str, const std::pair<standaloneFunc, std::string>& func, std::ostream& out){
		if(!func.first(str)){
			out<<"Failed test \""<<func.second<<"\"! (str = "<<str<<")"<<std::endl;
			return TEST_FAILED;
		}
		return TEST_SUCCEEDED;
	}

	void test(std::ostream* output = nullptr){
		std::ostream* out_ptr = &std::cout;
		if(output != nullptr)
			out_ptr = output;
		std::ostream& out = *out_ptr;

		for(int i=0;i<inputValues.size();i++){
			for(auto& stFunc : standaloneFuncs){
				auto result = performTest2(inputValues[i], stFunc, out);
				if(result == TEST_SUCCEEDED){
					succeeded++;
				}else if(result == TEST_FAILED){
					failed++;
				}
			}
			for(int j=0;j<inputValues.size();j++){
				for(int k=0;k<funcPairs.size();k++){
					auto result = performTest(inputValues[i], inputValues[j], funcPairs[k], out);
					if(result == TEST_SUCCEEDED){
						succeeded++;
					}else if(result == TEST_FAILED){
						failed++;
					}
				}
				for(int k=0;k<funcBoolPairs.size();k++){
					auto result = performCompTest(inputValues[i], inputValues[j], funcBoolPairs[k], out);
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
