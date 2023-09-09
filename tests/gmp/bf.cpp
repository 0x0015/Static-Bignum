#include <iostream>
#include <vector>
#include <future>
#include <thread>
#include <sstream>
#include "../../bf.hpp"
#include <gmpxx.h>
#include "Tester.hpp"

template<unsigned int man_size, unsigned int exp_size, unsigned int wordSize> std::string test(){
	std::stringstream output;
	output<<"bf<"<<man_size<<", "<<exp_size<<", "<<wordSize<<">"<<std::endl;
	Tester<mpf_class, bf<man_size, exp_size, wordSize>> tester;

	tester.inputValues.push_back("314159");
	tester.inputValues.push_back("314");
	tester.inputValues.push_back("1");
	//tester.inputValues.push_back("6436534235543543");
	//tester.inputValues.push_back("684387928375");
	//tester.inputValues.push_back("a68bc438d9ef8f75");
	//tester.inputValues.push_back("a68bc438d9ef64356437658676997666543642366578f75");
	tester.inputValues.push_back("2");
	//tester.inputValues.push_back("0");
	
	{
		auto tmp = tester.inputValues;
		for(const auto& o : tmp){
			tester.inputValues.push_back("-" + o);
		}
	}

	tester.autoGenerateBasicOpFuncs();
	tester.autoGenerateStrTest();
	/*
	typename decltype(tester)::funcPair plusFunc;
	plusFunc.t1Func = [](const auto& a, const auto& b){return a + b;};
	plusFunc.t2Func = [](const auto& a, const auto& b){return a + b;};
	plusFunc.name = "+";
	tester.funcPairs.push_back(plusFunc);
	*/
	
	tester.test(&output);

	return output.str();
}

int main(){
	std::vector<std::future<std::string>> outputs;
	//outputs.push_back(std::async(test<128, 128, 1>));
	//outputs.push_back(std::async(test<512, 512, 1>));
	//outputs.push_back(std::async(test<1024, 1>));takes a really long time
	//outputs.push_back(std::async(test<128, 128, 2>));
	//outputs.push_back(std::async(test<512, 512, 2>));
	//outputs.push_back(std::async(test<1024, 1024, 2>));
	//outputs.push_back(std::async(test<128, 128, 4>));
	//outputs.push_back(std::async(test<512, 512, 4>));
	//outputs.push_back(std::async(test<1024, 1024, 4>));
	std::cout<<test<128, 128, 4>()<<std::endl;

	for(auto& o : outputs){
		std::cout<<o.get();
	}
	return 0;
}
