#include "../bf.hpp"
#include <iomanip>

int main(){
	bn<512> a("314159");
	constexpr bn<512> b(0x314);
	std::cout<<a.to_string()<<std::endl;
	std::cout<<(a+b).to_string()<<std::endl;
	std::cout<<(a-b).to_string()<<std::endl;
	std::cout<<(a*b).to_string()<<std::endl;
	std::cout<<(a/b).to_string()<<std::endl;
	//bf<512> test;
	//std::cout<<bf<512>::getBasePower(4).to_string()<<std::endl;
	return 0;
}
