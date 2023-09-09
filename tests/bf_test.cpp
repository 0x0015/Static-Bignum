#include "../bf.hpp"

int main(){
	bf<128> test(4);
	std::cout<<test.to_string()<<std::endl;
	std::cout<<"man: "<<test.mantissa.to_string()<<std::endl;
	std::cout<<"exp: "<<test.exponent.to_string()<<std::endl;
	//auto res = test + test;
	//std::cout<<"man: "<<res.mantissa.to_string()<<std::endl;
	//std::cout<<"exp: "<<res.exponent.to_string()<<std::endl;
	//std::cout<<res.to_string()<<std::endl;
	return 0;
}
