#ifndef __BIGNUM_SIGNED_HPP__
#define __BIGNUM_SIGNED_HPP__

#include "bn.hpp"

template<unsigned int SIZE, unsigned int WORD_SIZE = fastestWordSize> class bn_s{//two's complement
public:
	using signed_array_type = intn<WORD_SIZE>;
	using signed_array_temp_type = intn<WORD_SIZE * 2>;
	bn<SIZE, WORD_SIZE> value;
	constexpr static unsigned int BN_ARRAY_SIZE = bn<SIZE, WORD_SIZE>::BN_ARRAY_SIZE;
	constexpr bn_s(){}
	constexpr bn_s(signed_array_temp_type val){
		from_int(val);
	}
	bn_s(const std::string& str){
		from_string(str);
	}
	constexpr void negate(){
		if(value.is_zero())
			return;
		value = ~value + bn<SIZE, WORD_SIZE>::one();
	}
	constexpr bool isNegative() const{
		return static_cast<signed_array_type>(value.array[BN_ARRAY_SIZE - 1]) < 0;//check most segnificant bit
	}
	constexpr void from_int(signed_array_temp_type i){
		if(i >= 0)
			value.from_int(i);
		else{
			value.from_int(std::abs(i));
			negate();
		}
	}
	void from_string(const std::string& str){
		if(str.size() == 0){
			value.from_zeros();
			return;
		}
		if(str[0] == '-'){
			value.from_string(str.substr(1, str.size()-1));
			negate();
		}else{
			value.from_string(str);
		}
	}
	constexpr signed_array_temp_type to_int() const{
		if(isNegative()){
			bn_s tmp = *this;
			tmp.negate();
			return -((signed_array_temp_type)tmp.value.to_int());
		}else
			return value.to_int();
	}
	std::string to_string() const{
		if(isNegative()){
			bn_s tmp = *this;
			tmp.negate();
			return "-" + tmp.value.to_string();
		}else{
			return value.to_string();
		}
	}
	constexpr bn<SIZE, WORD_SIZE>& to_bn(){
		return value;
	}

	constexpr static bn_s zero(){
		bn_s output;
		output.value.from_zeros();
		return output;
	}
	constexpr static bn_s one(){
		bn_s output(1);
		return output;
	}
	constexpr static bn_s max(){
		bn_s output;
		output.value.from_zeros();
		output.value = ~output.value;
		output.value = output.value / 2;
		return output;
	}
	constexpr static bn_s min(){
		return max() + one();
	}

	constexpr void abs(){
		if(isNegative())
			negate();
	}

	template<unsigned int NEW_SIZE> constexpr bn_s<NEW_SIZE, WORD_SIZE> resize() const{
		bn_s<NEW_SIZE, WORD_SIZE> output;
		if(isNegative()){
			bn_s tmp = *this;
			tmp.negate();
			output.value = tmp.value.template resize<NEW_SIZE>();
			output.negate();
		}else{
			output.value = value.template resize<NEW_SIZE>();
		}
		return output;
	}
	constexpr bn_s operator+(const bn_s& other) const{
		bn_s c;
		c.value = value + other.value;
		return c;
	}
	constexpr bn_s operator-(const bn_s& other) const{
		bn_s c;
		c.value = value - other.value;
		return c;
	}
	constexpr bn_s operator*(const bn_s& other) const{//for some reason there seems to be no need for sign jank as there is for division
		bn_s c;
		c.value = value * other.value;
		return c;
	} 
	constexpr bn_s operator/(const bn_s& other) const{
		bn_s a = *this;
		bn_s b = other;
		bool a_neg = a.isNegative();
		bool b_neg = b.isNegative();
		if(a_neg)
			a.negate();
		if(b_neg)
			b.negate();
		bn_s c;
		c.value = a.value / b.value;
		if(a_neg != b_neg)
			c.negate();
		return c;
	}
	constexpr bool operator<(const bn_s& other) const{
		bool thisNeg = isNegative();
		bool otherNeg = other.isNegative();
		if(thisNeg && !otherNeg)
			return true;
		if(otherNeg && !thisNeg)
			return false;
		if(thisNeg && otherNeg)
			return value > other.value;
		return value < other.value;
	}
	constexpr bool operator>(const bn_s& other) const{
		bool thisNeg = isNegative();
		bool otherNeg = other.isNegative();
		if(!thisNeg && otherNeg)
			return true;
		if(!otherNeg && thisNeg)
			return false;
		if(thisNeg && otherNeg)
			return value < other.value;
		return value > other.value;
	}
	constexpr bool operator==(const bn_s& other) const{
		return value == other.value;
	}
	constexpr bool operator!=(const bn_s& other) const{
		return value != other.value;
	}
	constexpr bool operator<=(const bn_s& other) const{
		return !(*this > other);
	}
	constexpr bool operator>=(const bn_s& other) const{
		return !(*this < other);
	}
	constexpr bn_s& operator++(int){
		value++;
		return *this;
	}
	constexpr bn_s& operator--(int){
		value--;
		return *this;
	}
	constexpr bn_s operator<<(int nbits) const{
		bn_s output = *this;
		if(output.isNegative()){
			output.negate();
			output.value = output.value << nbits;
			output.negate();
			return output;
		}else{
			output.value = output.value << nbits;
			return output;
		}
	}
	constexpr bn_s operator>>(int nbits) const{
		bn_s output = *this;
		if(output.isNegative()){
			output.negate();
			output.value = output.value >> nbits;
			output.negate();
			return output;
		}else{
			output.value = output.value >> nbits;
			return output;
		}
	}
};

#endif
