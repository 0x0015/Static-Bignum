#include <iostream>
#include <vector>
#include <future>
#include <thread>
#include <sstream>
#include "../../bn_s.hpp"
#include <gmpxx.h>
#include "Tester.hpp"

template<unsigned int size, unsigned int wordSize> std::string test(){
	std::stringstream output;
	output<<"bn_s<"<<size<<", "<<wordSize<<"> array size: "<<bn_s<size, wordSize>::BN_ARRAY_SIZE<<std::endl;
	Tester<mpz_class, bn_s<size, wordSize>> tester;

	tester.inputValues.push_back("314159");
	tester.inputValues.push_back("314");
	tester.inputValues.push_back("1");
	tester.inputValues.push_back("6436534235543543");
	tester.inputValues.push_back("684387928375");
	tester.inputValues.push_back("a68bc438d9ef8f75");
	if(size > 32)
		tester.inputValues.push_back("a68bc438d9ef64356437658676997666543642366578f75");
	tester.inputValues.push_back("2");
	tester.inputValues.push_back("0");
	
	{
		auto tmp = tester.inputValues;
		for(const auto& o : tmp){
			if(o != "0")
				tester.inputValues.push_back("-" + o);
		}
	}

	tester.autoGenerateBasicOpFuncs();
	tester.autoGenerateStrTest();
	tester.autoGenerateBasicComparisonFuncs();
	
	tester.test(&output);

	return output.str();
}

int main(){
	std::vector<std::future<std::string>> outputs;
	outputs.push_back(std::async(test<32, 1>));
	outputs.push_back(std::async(test<64, 1>));
	outputs.push_back(std::async(test<128, 1>));
	outputs.push_back(std::async(test<512, 1>));
	//outputs.push_back(std::async(test<1024, 1>));takes a really long time
	outputs.push_back(std::async(test<32, 2>));
	outputs.push_back(std::async(test<64, 2>));
	outputs.push_back(std::async(test<128, 2>));
	outputs.push_back(std::async(test<512, 2>));
	outputs.push_back(std::async(test<1024, 2>));
	outputs.push_back(std::async(test<32, 4>));
	outputs.push_back(std::async(test<64, 4>));
	outputs.push_back(std::async(test<128, 4>));
	outputs.push_back(std::async(test<512, 4>));
	outputs.push_back(std::async(test<1024, 4>));

	for(auto& o : outputs){
		std::cout<<o.get();
	}
	return 0;
}
