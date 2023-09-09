#ifndef __BIGFLOAT_HPP__
#define __BIGFLOAT_HPP__

#include "bn_s.hpp"
#include <array>

template<unsigned int MAN_SIZE, unsigned int EXP_SIZE = MAN_SIZE, unsigned int WORD_SIZE = fastestWordSize> class bf{
public:
	bn_s<MAN_SIZE, WORD_SIZE> mantissa;
	bn_s<EXP_SIZE, WORD_SIZE> exponent;
	constexpr bf(){}
	constexpr bf(const bn_s<MAN_SIZE, WORD_SIZE>& man, const bn_s<EXP_SIZE, WORD_SIZE>& exp){
		mantissa = man;
		exponent = exp;
		mantissaNorm();
	}
	constexpr bf(const bn_s<MAN_SIZE, WORD_SIZE>& val){
		from_int(val);
	}
	bf(const std::string& str){
		from_string(str);
	}

	constexpr void from_int(const bn_s<MAN_SIZE, WORD_SIZE>& i){
		mantissa = i;
		exponent = bn_s<EXP_SIZE, WORD_SIZE>::zero();
		mantissaNorm();
	}
	void from_string(const std::string& str){
		auto dotPos = str.find('.');
		if(dotPos == std::string::npos){
			from_int(bn_s<MAN_SIZE, WORD_SIZE>(str));
			return;
		}
		std::string noDot = str.substr(0, dotPos) + str.substr(dotPos+1, str.length()-dotPos-1);
		int predot = dotPos;
		int pastDot = str.length() - dotPos-1;
		//std::cout<<noDot<<" "<<predot<<" "<<pastDot<<std::endl;
		mantissa.from_string(noDot);
		exponent.from_int(-pastDot * 4);
		mantissaNorm();
	}
	std::string to_string() const{
		bf tmp = *this;
		if(tmp.mantissa.value.is_zero())
			return "0";
		//adjust the exponent such that it's a multiple of 4 -> the whole number is a multiple of 16 (0x10)
		int tmpexp = tmp.exponent.to_int();
		int exp16 = tmpexp % 4;
		int newexp = tmpexp - exp16;
		/*
		if(tmpexp < 0){
			newexp = tmpexp + (2 + exp16);
		}else{
			newexp = tmpexp - (2 - exp16);
		}
		*/
		tmp.matchExponent(newexp);
		//std::cout<<"exponent: "<<tmp.exponent.to_string()<<std::endl;
		std::string mantissaStr = tmp.mantissa.to_string();
		int dotPos = (int)mantissaStr.length() - (std::abs(tmp.exponent.to_int()) / 4);
		if(dotPos < 0){
			//return "unable to calculate";
			if(tmpexp < 0){
				std::string zeropad = "";
				for(int i=dotPos;i<=0;i++)
					zeropad += '0';
				mantissaStr = zeropad + mantissaStr;
			}else{
				for(int i=dotPos;i<=0;i++)
					mantissaStr += '0';
			}
			dotPos = 0;
		}
		//std::cout<<dotPos<<std::endl;
		std::string out = mantissaStr.substr(0, dotPos) + "." + mantissaStr.substr(dotPos, mantissaStr.length()-dotPos);
		//std::cout<<"str no dot: "<<out<<std::endl;
		//remove trailing 0s
		int lastZero = out.length()-1;
		for(;lastZero>=0;lastZero--){
			if(out[lastZero] != '0')
				break;
		}
		out = out.substr(0, lastZero+1);
		//remove trailing .
		if(out[out.length()-1] == '.')
			out = out.substr(0, out.length()-1);

		return out;
	}
	constexpr bn_s<MAN_SIZE, WORD_SIZE> to_int() const{
		bf tmp = *this;
		tmp.matchExponent(bn_s<EXP_SIZE, WORD_SIZE>::zero());
		return tmp.mantissa;
	}

	constexpr static bf zero(){
		bf output;
		output.mantissa.value.from_zeros();
		output.exponent.value.from_zeros();
		return output;
	}
	constexpr static bf posInf(){
		bf output;
		output.mantissa.value.from_zeros();
		output.exponent = bn_s<EXP_SIZE, WORD_SIZE>::max();
		return output;
	}
	constexpr static bf negInf(){
		bf output;
		output.mantissa.value.from_zeros();
		output.exponent = bn_s<EXP_SIZE, WORD_SIZE>::min();
		return output;
	}
	constexpr static bf getNan(){
		bf output;
		output.exponent = bn_s<EXP_SIZE, WORD_SIZE>::max();
		output.mantissa = bn_s<MAN_SIZE, WORD_SIZE>::one();
		return output;
	}
	constexpr bool isNan(){
		return (exponent.value == bn_s<EXP_SIZE, WORD_SIZE>::max()) && (mantissa != bn_s<MAN_SIZE, WORD_SIZE>::zero());
	}
//private:
	constexpr bool shouldBeZero(){
		if(exponent == bn_s<EXP_SIZE, WORD_SIZE>::min() ||  exponent == bn_s<EXP_SIZE, WORD_SIZE>::max())
			return false;//if it's nan or inf then it's not zero
		return mantissa == bn_s<MAN_SIZE, WORD_SIZE>::zero();
	}

	constexpr void baseShift(const bn_s<EXP_SIZE, WORD_SIZE>& n){
		exponent = exponent + n;
		auto n_abs = n;
		n_abs.abs();
		bn_s<EXP_SIZE, WORD_SIZE> msize(MAN_SIZE * 8);
		if(n.isNegative()){
			if(n_abs > msize)
				mantissa = bn_s<MAN_SIZE, WORD_SIZE>::zero();
			else{
				mantissa = mantissa << (int)n_abs.value.to_int32();
				//mantissa = mantissa * getBasePower(n_abs.to_int());
			}
		}else{
			if(n_abs > msize)
				*this = bf::posInf();
			else{
				mantissa = mantissa >> (int)n_abs.value.to_int32();
				//mantissa = mantissa / getBasePower(n_abs.to_int());
			}
		}
	}
public:
	constexpr unsigned int baseExpsInMantissa() const{//really quite slow, could use optimization
		unsigned int r = 0;
		bn_s<MAN_SIZE, WORD_SIZE> tmp = mantissa;
		if(tmp.isNegative())//not sure if this is strictly neccesary
			tmp.negate();
		for(;r<MAN_SIZE * 8 + 1;r++){
			tmp.value = tmp.value >> 1;
			if(tmp.value.is_zero()){
				break;
			}
		}
		return r;
	}
	constexpr void matchExponent(const bn_s<EXP_SIZE, WORD_SIZE>& n){
		bn_s<EXP_SIZE, WORD_SIZE> exp_diff = n - exponent;
		baseShift(exp_diff);
	}
	constexpr static unsigned int maxExponentInMantissa(){
		return (MAN_SIZE * 8) - 3;//should be -1, but seems to do funky things with -1 or -2
	}
	constexpr void mantissaBaseExpMatch(unsigned int toMatch){
		int mantissa_exp_diff = (int)toMatch - (int)baseExpsInMantissa();
		//std::cout<<"mantissaBaseExpMatch: shifting by: "<<mantissa_exp_diff<<std::endl;
		baseShift(bn_s<EXP_SIZE, WORD_SIZE>(-mantissa_exp_diff));
	}
	constexpr void mantissaNorm(){
		mantissaBaseExpMatch(maxExponentInMantissa());
	}
public:
	constexpr bool operator==(const bf& other) const{
		return mantissa == other.mantissa && exponent == other.exponent;
	}
	constexpr bool operator!=(const bf& other) const{
		return mantissa != other.mantissa || exponent != other.exponent;
	}
	constexpr static std::pair<bf, const bf&> bfMatchLargeExponent(const bf& f1, const bf& f2){
		if(f1.exponent > f2.exponent){
			bf tmp = f2;
			tmp.matchExponent(f1.exponent);
			return std::pair<bf, const bf&>(tmp, f1);
		}else if(f2.exponent > f1.exponent){
			bf tmp = f1;
			tmp.matchExponent(f2.exponent);
			return std::pair<bf, const bf&>(tmp, f2);
		}else{
			return std::pair<bf, const bf&>(f1, f2);
		}
	}
	constexpr static std::pair<bf, const bf&> bfMatchSmallExponent(const bf& f1, const bf& f2){
		if(f1.exponent < f2.exponent){
			bf tmp = f2;
			tmp.matchExponent(f1.exponent);
			return std::pair<bf, const bf&>(tmp, f1);
		}else if(f2.exponent < f1.exponent){
			bf tmp = f1;
			tmp.matchExponent(f2.exponent);
			return std::pair<bf, const bf&>(tmp, f2);
		}else{
			return std::pair<bf, const bf&>(f1, f2);
		}
	}
	constexpr static std::pair<bf, bf> bfMatchLargeExponentRetainOrder(const bf& f1, const bf& f2){
		if(f1.exponent > f2.exponent){
			bf tmp = f2;
			tmp.matchExponent(f1.exponent);
			return std::pair<bf, bf>(f1, tmp);
		}else if(f2.exponent > f1.exponent){
			bf tmp = f1;
			tmp.matchExponent(f2.exponent);
			return std::pair<bf, const bf&>(tmp, f2);
		}else{
			return std::pair<bf, const bf&>(f1, f2);
		}
	}
	constexpr static std::pair<bf, bf> bfMatchSmallExponentRetainOrder(const bf& f1, const bf& f2){
		if(f1.exponent < f2.exponent){
			bf tmp = f2;
			tmp.matchExponent(f1.exponent);
			return std::pair<bf, bf>(f1, tmp);
		}else if(f2.exponent < f1.exponent){
			bf tmp = f1;
			tmp.matchExponent(f2.exponent);
			return std::pair<bf, const bf&>(tmp, f2);
		}else{
			return std::pair<bf, const bf&>(f1, f2);
		}
	}
	constexpr bf operator+(const bf& other) const{
		std::pair<bf, const bf&> exponentMatched = bfMatchSmallExponent(*this, other);
		bf output;
		output.exponent = exponentMatched.first.exponent;
		output.mantissa = exponentMatched.first.mantissa + exponentMatched.second.mantissa;
		//std::cout<<output.mantissa.to_string()<<std::endl;
		//std::cout<<output.exponent.to_string()<<std::endl;
		//std::cout<<output.to_string()<<std::endl;
		output.mantissaNorm();
		return output;
	}
	constexpr bf operator-(const bf& other) const{
		std::pair<bf, bf> exponentMatched = bfMatchSmallExponentRetainOrder(*this, other);
		bf output;
		output.exponent = exponentMatched.first.exponent;
		output.mantissa = exponentMatched.first.mantissa - exponentMatched.second.mantissa;
		output.mantissaNorm();
		return output;
	}
	constexpr bf operator*(const bf& other) const{
		bn_s<MAN_SIZE * 2, WORD_SIZE> matchMan1 = mantissa.template resize<MAN_SIZE * 2>();
		bn_s<MAN_SIZE * 2, WORD_SIZE> matchMan2 = other.mantissa.template resize<MAN_SIZE * 2>();
		bf<MAN_SIZE * 2, EXP_SIZE, WORD_SIZE> o2;
		o2.exponent = exponent + other.exponent;
		o2.mantissa = matchMan1 * matchMan2;
		//std::cout<<matchMan1.to_string()<<std::endl;
		//std::cout<<matchMan2.to_string()<<std::endl;
		//std::cout<<o2.exponent.to_string()<<std::endl;
		//std::cout<<o2.mantissa.to_string()<<std::endl;
		//std::cout<<o2.to_int().to_string()<<std::endl;
		//std::cout<<"mbem"<<std::endl;
		o2.mantissaBaseExpMatch(maxExponentInMantissa());
		//std::cout<<o2.exponent.to_string()<<std::endl;
		//std::cout<<o2.mantissa.to_string()<<std::endl;
		//std::cout<<o2.to_int().to_string()<<std::endl;
		bf output;
		output.exponent = o2.exponent;
		output.mantissa = o2.mantissa.template resize<MAN_SIZE>();
		//output.mantissaNorm();//theoretically shouldn't be neccesary, as o2.mantissaBaseExpMatch... does this already
		//std::cout<<output.mantissa.to_string()<<std::endl;
		//std::cout<<output.exponent.to_string()<<std::endl;
		return output;
	}
	constexpr bf operator/(const bf& other) const{
		bf<MAN_SIZE * 2, EXP_SIZE, WORD_SIZE> match1;
		bf<MAN_SIZE * 2, EXP_SIZE, WORD_SIZE> match2;
		match1.mantissa = mantissa.template resize<MAN_SIZE * 2>();
		match1.exponent = exponent;
		match2.mantissa = other.mantissa.template resize<MAN_SIZE * 2>();
		match2.exponent = other.exponent;
		match1.mantissaNorm();
		bf<MAN_SIZE * 2, EXP_SIZE, WORD_SIZE> o2;
		o2.exponent = match1.exponent - match2.exponent;
		o2.mantissa = match1.mantissa / match2.mantissa;
		o2.mantissaBaseExpMatch(maxExponentInMantissa());
		bf output;
		output.exponent = o2.exponent;
		output.mantissa = o2.mantissa.template resize<MAN_SIZE>();
		return output;
	}
};

#endif
