#pragma once
#include <iostream>
#include <vector>
#include <functional>

template<typename T1, typename T2> class Tester_Func{
public:
	std::string funcName;
	std::string testName;
	std::function<void(T1&, T1&, T1&)> T1func;
	std::function<void(T2&, T2&, T2&)> T2func;
};

template<typename T1, typename T2> class Tester_Result{
public:
	bool succeeded;
	std::string funcName;
	std::string testName;
	std::pair<T1, T1> T1input;
	T1 T1output;
	std::pair<T2, T2> T2input;
	T2 T2output;
};

template<typename T1, typename T2> class Tester{
public:
	bool saveSuccess = false;
	bool saveFailure = false;
	unsigned int numRandom = 10000;
	std::vector<T2> preDetermined;
	unsigned int numPreDetermined = 1000;
	bool preDeterminedLHS = true;
	bool preDeterminedRHS = true;
	std::vector<Tester_Func<T1, T2>> testFuncs;
	std::function<void(T1&, T2&)> T2ToT1;
	std::function<void(T2&, T1&)> T1ToT2;
	std::function<void(T1&, T1&)> T1assign;
	std::function<void(T2&, T2&)> T2assign;
	std::function<bool(T2&, T2&)> compare;
	std::vector<Tester_Result<T1, T2>> outputs;
	unsigned int numSucceeded = 0;
	unsigned int numFailed = 0;
	bool randomSeed = false;
	std::function<void(T2&)> getRandomT2;
	std::function<void(T1&)> printT1;
	std::function<void(T2&)> printT2;
	void test(){
		if(randomSeed){
			srand(time(0));
		}else{
			srand(0);
		}
		T1 a1, b1, c1;
		T2 a2, b2, c2;
		T2 c1Conv;
		for(auto& o : testFuncs){
			if(preDeterminedLHS){ for(unsigned int i=0;i<preDetermined.size();i++){
				a2 = preDetermined[i];
				T2ToT1(a1, a2);
				for(unsigned int j=0;j<numPreDetermined;j++){
					getRandomT2(b2);
					T2ToT1(b1, b2);
					o.T1func(a1, b1, c1);
					o.T2func(a2, b2, c2);
					T1ToT2(c1Conv, c1);
					if(compare(c1Conv, c2)){
						if(saveSuccess){
							outputs.push_back(Tester_Result<T1, T2>{true, o.funcName, o.testName, {a1, b1}, c1, {a2, b2}, c2});
						}
						numSucceeded++;
					}else{
						if(saveFailure){
							outputs.push_back(Tester_Result<T1, T2>{false, o.funcName, o.testName, {a1, b1}, c1, {a2, b2}, c2});
						}
						numFailed++;
					}
				}
			}}
			if(preDeterminedRHS){ for(unsigned int i=0;i<preDetermined.size();i++){
				b2 = preDetermined[i];
				T2ToT1(b1, b2);
				for(unsigned int j=0;j<numPreDetermined;j++){
					getRandomT2(a2);
					T2ToT1(a1, a2);
					o.T1func(a1, b1, c1);
					o.T2func(a2, b2, c2);
					T1ToT2(c1Conv, c1);
					if(compare(c1Conv, c2)){
						if(saveSuccess){
							outputs.push_back(Tester_Result<T1, T2>{true, o.funcName, o.testName, {a1, b1}, c1, {a2, b2}, c2});
						}
						numSucceeded++;
					}else{
						if(saveFailure){
							outputs.push_back(Tester_Result<T1, T2>{false, o.funcName, o.testName, {a1, b1}, c1, {a2, b2}, c2});
						}
						numFailed++;
					}
				}
			}}
			for(unsigned int i=0;i<numRandom;i++){
				getRandomT2(a2);
				getRandomT2(b2);
				T2ToT1(a1, a2);
				T2ToT1(b1, b2);
				o.T1func(a1, b1, c1);
				o.T2func(a2, b2, c2);
				T1ToT2(c1Conv, c1);
				if(compare(c1Conv, c2)){
					if(saveSuccess){
						outputs.push_back(Tester_Result<T1, T2>{true, o.funcName, o.testName, {a1, b1}, c1, {a2, b2}, c2});
					}
					numSucceeded++;
				}else{
					if(saveFailure){
						outputs.push_back(Tester_Result<T1, T2>{false, o.funcName, o.testName, {a1, b1}, c1, {a2, b2}, c2});
					}
					numFailed++;
				}
			}
		}
	}
};
