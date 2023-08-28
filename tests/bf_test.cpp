#include "../bf.hpp"

int main(){
	bf<128> test(0x20);
	std::cout<<"initial"<<std::endl;
	std::cout<<test.mantissa.to_string()<<std::endl;
	std::cout<<test.exponent.to_string()<<std::endl;
	std::cout<<test.baseExpsInMantissa()<<std::endl;
	test.mantissaNorm();
	std::cout<<"test1"<<std::endl;
	std::cout<<test.mantissa.to_string()<<std::endl;
	std::cout<<test.exponent.to_string()<<std::endl;
	std::cout<<test.baseExpsInMantissa()<<std::endl;
	test.matchExponent(0);
	std::cout<<"test2"<<std::endl;
	std::cout<<test.mantissa.to_string()<<std::endl;
	std::cout<<test.exponent.to_string()<<std::endl;
	std::cout<<test.baseExpsInMantissa()<<std::endl;
	return 0;
}
