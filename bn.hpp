#ifndef __BIGNUM_HPP__
#define __BIGNUM_HPP__

#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>
#include "builtin_types.hpp"

template<unsigned int SIZE, unsigned int WORD_SIZE = fastestWordSize> class bn
{
public:
	using array_type = uintn<WORD_SIZE>;
	using array_temp_type = uintn<WORD_SIZE * 2>;
	constexpr static unsigned int BN_ARRAY_SIZE = divOrZero(SIZE, WORD_SIZE);
	array_type array[BN_ARRAY_SIZE];
	constexpr bn() : array(){
	}
	constexpr bn(array_temp_type val) : array(){
		from_int(val);
	}
	bn(const std::string& str){
		from_string(str);
	}
	constexpr void from_zeros(){
		for(unsigned int i=0;i<BN_ARRAY_SIZE;i++){
			array[i] = 0;
		}
	}
	constexpr void from_int(array_temp_type i){
		from_zeros();
		/* Endianness issue if machine is not little-endian? */
		if constexpr(WORD_SIZE == 1){
		  array[0] = (i & 0x000000ff);
		  array[1] = (i & 0x0000ff00) >> 8;
		  array[2] = (i & 0x00ff0000) >> 16;
		  array[3] = (i & 0xff000000) >> 24;
		}
		if constexpr(WORD_SIZE == 2){
		  array[0] = (i & 0x0000ffff);
		  array[1] = (i & 0xffff0000) >> 16;
		}
		if constexpr(WORD_SIZE == 4){
		  array[0] = i;
		  array_temp_type num_32 = 32;
		  array_temp_type tmp = i >> num_32; /* bit-shift with U64 operands to force 64-bit results */
		  array[1] = tmp;
		}
	}
private:
	constexpr static uint8_t char2int(char input){
		if (input <= '9' && input >= '0') return (uint8_t)input - '0';
		if (input <= 'F' && input >= 'A') return (uint8_t)input - 'A' + 10;
		if (input <= 'f' && input >= 'a') return (uint8_t)input - 'a' + 10;
		return 0xFF;
	}
	constexpr static uint8_t fromChars(char c1, char c2){
		return char2int(c1) * 16 + char2int(c2);
	}
	constexpr static char indexOrZero(const std::string& str, int index){
		if(index >= 0 && index < str.size())
			return str[index];
		return '0';
	}
public:
	void from_string(const std::string& str){
		from_zeros();
		int currentWord = 0;
		for(int i=str.size()-((WORD_SIZE * 2));i>=((-2*(int)WORD_SIZE)+1);i-=WORD_SIZE * 2){
			if constexpr(WORD_SIZE == 1){
				uint8_t num = fromChars(indexOrZero(str, i+0), indexOrZero(str, i+1));
				array[currentWord] = num;
			}
			if constexpr(WORD_SIZE == 2){
				uint8_t num1 = fromChars(indexOrZero(str, i+0), indexOrZero(str, i+1));
				uint8_t num2 = fromChars(indexOrZero(str, i+2), indexOrZero(str, i+3));
				array[currentWord] = fromint1(num1, num2);
			}
			if constexpr(WORD_SIZE == 4){
				uint8_t num1 = fromChars(indexOrZero(str, i+0), indexOrZero(str, i+1));
				uint8_t num2 = fromChars(indexOrZero(str, i+2), indexOrZero(str, i+3));
				uint8_t num3 = fromChars(indexOrZero(str, i+4), indexOrZero(str, i+5));
				uint8_t num4 = fromChars(indexOrZero(str, i+6), indexOrZero(str, i+7));
				array[currentWord] = fromint1(num1, num2, num3, num4);
			}
			currentWord++;
		}
	}
	constexpr uint32_t to_int32() const{
	
 		 uint32_t ret = 0;
 		
 		 /* Endianness issue if machine is not little-endian? */
		 if constexpr(WORD_SIZE == 1){
 		 	ret += array[0];
 		 	ret += array[1] << 8;
 		 	ret += array[2] << 16;
 		 	ret += array[3] << 24;
		 }
		 if constexpr(WORD_SIZE == 2){
 		 	ret += array[0];
 		 	ret += array[1] << 16;
		 }
		 if constexpr(WORD_SIZE == 4){
 		 	ret += array[0];
		 }
 		
 		 return ret;
	}
	constexpr array_type to_int() const{
		return array[0];
	}
private:
	static std::string hexStr(array_type num){
		return (std::stringstream()<<std::hex<<std::setfill('0')<<std::setw(sizeof(array_type) * 2)<<(int)num).str();
	}
public:
	std::string to_string() const{
		std::string str;
		for(int i=BN_ARRAY_SIZE-1;i>=0;i--){
			//std::cout<<std::hex<<std::setfill('0')<<std::setw(2)<<(int)array[i]<<std::endl;
			str += hexStr(array[i]);
		}
		int leadingZeros = 0;
		for(int i=0;i<str.size();i++){
			if(str[i] != '0')
				break;
			leadingZeros++;
		}
		if(leadingZeros == str.size())
			return "0";
		if(leadingZeros > 0)
			return str.substr(leadingZeros, str.size()-leadingZeros);
		return str;
	}
	constexpr bool is_zero() const{
		for(int i=0;i<BN_ARRAY_SIZE;i++){
			if(array[i] != 0)
				return false;
		}
		return true;
	}
	constexpr static bn zero(){
		bn output;
		output.from_zeros();
		return output;
	}
	constexpr static bn one(){
		bn output(1);
		return output;
	}
	constexpr static bn max(){
		return ~zero();
	}
	void from_random(){
		for(int i=0;i<BN_ARRAY_SIZE;i++){
			array[i] = rand();
		}
	}
	template<unsigned int NEW_SIZE> constexpr bn<NEW_SIZE, WORD_SIZE> resize() const{
		bn<NEW_SIZE, WORD_SIZE> output;
		output.from_zeros();
		for(int i=0;(i<BN_ARRAY_SIZE && i<bn<NEW_SIZE, WORD_SIZE>::BN_ARRAY_SIZE);i++){
			output.array[i] = array[i];
		}
		return output;
	}
	constexpr bn operator+(const bn& other) const{	
		array_temp_type tmp{};
		int carry = 0;
		bn c;
		for (int i = 0; i < BN_ARRAY_SIZE; ++i)
		{
		  tmp = (array_temp_type)array[i] + other.array[i] + carry;
		  carry = (tmp > std::numeric_limits<array_type>::max());
		  c.array[i] = (tmp & std::numeric_limits<array_type>::max());
		}
		return c;
	}
	constexpr bn operator-(const bn& other) const{
		array_temp_type res{};
		array_temp_type tmp1{};
		array_temp_type tmp2{};
		int borrow = 0;
		bn c;
		for (int i = 0; i < BN_ARRAY_SIZE; ++i)
		{
		  tmp1 = (array_temp_type)array[i] + ((array_temp_type)std::numeric_limits<array_type>::max() + 1); /* + number_base */
		  tmp2 = (array_temp_type)other.array[i] + borrow;
		  res = (tmp1 - tmp2);
		  c.array[i] = (array_type)(res & std::numeric_limits<array_type>::max()); /* "modulo number_base" == "% (number_base - 1)" if number_base is 2^N */
		  borrow = (res <= std::numeric_limits<array_type>::max());
		}
		return c;
	}
	constexpr bn operator*(const bn& other) const{
		bn row;
		bn tmp;
		bn c;
		c.from_zeros();
		for (int i = 0; i < BN_ARRAY_SIZE; ++i)
		{
		  row.from_zeros();
		  for (int j = 0; j < BN_ARRAY_SIZE; ++j)
		  {
		    if (i + j < BN_ARRAY_SIZE)
		    {
		      tmp.from_zeros();
		      array_temp_type intermediate = ((array_temp_type)array[i] * (array_temp_type)other.array[j]);
		      tmp.from_int(intermediate);
		      tmp.lshift_word(i + j);
		      row = tmp + row;
		    }
		  }
		  c = c + row;
		}
		return c;
	}
	/* Tokens returned by bignum_cmp() for value comparison */
	enum { SMALLER = -1, EQUAL = 0, LARGER = 1 };
	constexpr int cmp(const bn& other) const{
		int i = BN_ARRAY_SIZE;
		do
		{
		  i -= 1; /* Decrement first, to start with last array element */
		  if (array[i] > other.array[i])
		  {
		    return LARGER;
		  }
		  else if (array[i] < other.array[i])
		  {
		    return SMALLER;
		  }
		}
		while (i != 0);
		return EQUAL;
	}
	constexpr bool equal(const bn& other) const{
		for(int i=0;i<BN_ARRAY_SIZE;i++){
			if(array[i] != other.array[i])
				return false;
		}
		return true;
	}
	constexpr bool operator<(const bn& other) const{
		return cmp(other) == SMALLER;
	}
	constexpr bool operator>(const bn& other) const{
		return cmp(other) == LARGER;
	}
	constexpr bool operator==(const bn& other) const{
		return equal(other);
	}
	constexpr bool operator!=(const bn& other) const{
		return !(*this == other);
	}
	constexpr bool operator<=(const bn& other) const{
		return cmp(other) != LARGER;
	}
	constexpr bool operator>=(const bn& other) const{
		return cmp(other) != SMALLER;
	}
	constexpr bn operator/(const bn& other) const{
		if(other.is_zero()){
			bn c;
			c.from_zeros();
			return c;
		}
		bn current;
		bn denom;
		bn tmp;
		bn c;
		current.from_int(1);
		denom = other;
		tmp = *this;
		const array_type half_max = 1 + (array_type)(std::numeric_limits<array_type>::max() / 2);
		bool overflow = false;
		while (denom <= *this)     // while (denom <= a) {
		{
		  if (denom.array[BN_ARRAY_SIZE - 1] >= half_max)
		  {
		    overflow = true;
		    break;
		  }
		  current.lshift_one_bit();                //   current <<= 1;
		  denom.lshift_one_bit();                  //   denom <<= 1;
		}
		if (!overflow)
		{
		  denom.rshift_one_bit();                  // denom >>= 1;
		  current.rshift_one_bit();                // current >>= 1;
		}
		c.from_zeros();                             // int answer = 0;
		while (!current.is_zero())           // while (current != 0)
		{
		  if (tmp >= denom)  //   if (dividend >= denom)
		  {
		    tmp = tmp - denom;         //     dividend -= denom;
		    c = c | current;              //     answer |= current;
		  }
		  current.rshift_one_bit();                //   current >>= 1;
		  denom.rshift_one_bit();                  //   denom >>= 1;
		}                                           // return answer;
		return c;
	}
	constexpr std::pair<bn, bn> divmod(const bn& other) const{
		bn c, d;
		bn tmp;
		
		c = (*this / other);
		tmp = (c * other);	
		c = *this - tmp;

		return {c, d};
	}
	constexpr bn operator%(const bn& other) const{
		return divmod(other).second;
	}
	constexpr bn operator&(const bn& other) const{
		bn c;
		for (int i = 0; i < BN_ARRAY_SIZE; ++i)
		{
		  c.array[i] = (array[i] & other.array[i]);
		}
		return c;
	}
	constexpr bn operator|(const bn& other) const{
		bn c;
		for (int i = 0; i < BN_ARRAY_SIZE; ++i)
		{
		  c.array[i] = (array[i] | other.array[i]);
		}
		return c;
	}
	constexpr bn operator^(const bn& other) const{
		bn c;
		for (int i = 0; i < BN_ARRAY_SIZE; ++i)
		{
		  c.array[i] = (array[i] ^ other.array[i]);
		}
		return c;
	}
	constexpr bn operator~(){
		bn c;
		for(int i=0;i<BN_ARRAY_SIZE;i++){
			c.array[i] = ~array[i];
		}
		return c;
	}
	constexpr bn& operator++(int){
		array_type res{};
		array_temp_type tmp{}; /* copy of n */
		for (int i = 0; i < BN_ARRAY_SIZE; ++i)
		{
		  tmp = array[i];
		  res = tmp + 1;
		  array[i] = res;
		  if (res > tmp)
		  {
		    break;
		  }
		}
		return *this;
	}
	constexpr bn& operator--(int){
		array_type tmp{}; /* copy of n */
		array_type res{};
		for (int i = 0; i < BN_ARRAY_SIZE; ++i)
		{
		  tmp = array[i];
		  res = tmp - 1;
		  array[i] = res;
				  if (!(res > tmp))
		  {
		    break;
		  }
		}
		return *this;
	}
	constexpr bn operator<<(int nbits) const{
		bn b = *this;
		/* Handle shift in multiples of word-size */
		const int nbits_pr_word = (WORD_SIZE * 8);
		int nwords = nbits / nbits_pr_word;
		if (nwords != 0)
		{
		  b.lshift_word(nwords);
		  nbits -= (nwords * nbits_pr_word);
		}
				if (nbits != 0)
		{
		  int i = (BN_ARRAY_SIZE - 1);
		  for (; i > 0; --i)
		  {
		    b.array[i] = (b.array[i] << nbits) | (b.array[i - 1] >> ((8 * WORD_SIZE) - nbits));
		  }
		  b.array[i] <<= nbits;
		}
		return b;
	}
	constexpr bn operator>>(int nbits) const{
		bn b = *this;
		const int nbits_pr_word = (WORD_SIZE * 8);
		int nwords = nbits / nbits_pr_word;
		if (nwords != 0)
		{
		  b.rshift_word(nwords);
		  nbits -= (nwords * nbits_pr_word);
		}
				if (nbits != 0)
		{
		  int i = 0;
		  for (; i < (BN_ARRAY_SIZE - 1); ++i)
		  {
		    b.array[i] = (b.array[i] >> nbits) | (b.array[i + 1] << ((8 * WORD_SIZE) - nbits));
		  }
		  b.array[i] >>= nbits;
		}
		return b;
	}
	constexpr bn pow(const bn& other) const{
	  bn c;
	  bn tmp;
	
	  c.from_zeros();
	
	  if (other.is_zero())
	  {
	    /* Return 1 when exponent is 0 -- n^0 = 1 */
	    c++;
	  }
	  else
	  {
	    bn bcopy;
	    bcopy = other;
	
	    /* Copy a -> tmp */
	    tmp = *this;
	
	    bcopy--;
	 
	    /* Begin summing products: */
	    while (!bcopy.is_zero())
	    {
	
	      /* c = tmp * tmp */
	      c = *this * tmp;
	      /* Decrement b by one */
	      bcopy--;
	
	      tmp = c;
	    }
	
	    /* c = tmp */
	    c = tmp;
	  }
	  return c;
	}
private:
	constexpr void lshift_one_bit(){
		for (int i = (BN_ARRAY_SIZE - 1); i > 0; --i)
		{
		  array[i] = (array[i] << 1) | (array[i - 1] >> ((8 * WORD_SIZE) - 1));
		}
		array[0] <<= 1;
	}
	constexpr void rshift_one_bit(){
		for (int i = 0; i < (BN_ARRAY_SIZE - 1); ++i)
		{
		  array[i] = (array[i] >> 1) | (array[i + 1] << ((8 * WORD_SIZE) - 1));
		}
		array[BN_ARRAY_SIZE - 1] >>= 1;
	}
	constexpr void lshift_word(unsigned int nwords){
		if(nwords == 0)
			return;
		int i = (BN_ARRAY_SIZE - 1);
		/* Shift whole words */
		for (; i >= nwords; --i)
		{
		  array[i] = array[i - nwords];
		}
		/* Zero pad shifted words. */
		for (; i >= 0; --i)
		{
		  array[i] = 0;
		}
	}
	constexpr void rshift_word(unsigned int nwords){
		if(nwords == 0)
			return;

		if (nwords >= BN_ARRAY_SIZE)
		{
		  for (int i = 0; i < BN_ARRAY_SIZE; ++i)
		  {
		    array[i] = 0;
		  }
		  return;
		}
		int i = 0;
		for (; i < BN_ARRAY_SIZE - nwords; ++i)
		{
		  array[i] = array[i + nwords];
		}
		for (; i < BN_ARRAY_SIZE; ++i)
		{
		  array[i] = 0;
		}
	}
};


#endif /* #ifndef __BIGNUM_HPP__ */


