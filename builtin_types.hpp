#ifndef BUILTIN_TYPES_HPP
#define BUILTIN_TYPES_HPP


#ifndef BN_NO_STDINT
#include <stdint.h>
#else
#include "custom_stdint.h"
#endif

template<unsigned int N>
struct uintn_t{
	typedef void type;
};

template<>
struct uintn_t<1>{
	typedef uint8_t type;
};
template<>
struct uintn_t<2>{
	typedef uint16_t type;
};
template<>
struct uintn_t<4>{
	typedef uint32_t type;
};
template<>
struct uintn_t<8>{
	typedef uint64_t type;
};

template<unsigned int N> using uintn = typename uintn_t<N>::type;

constexpr inline uintn<2> fromint1(uintn<1> a, uintn<1> b){
	uintn<2> output = b + (a << 8);
	return output;
}

constexpr inline uintn<4> fromint1(uintn<1> a, uintn<1> b, uintn<1> c, uintn<1> d){
	uintn<4> output = d + (c << 8) + (b << 16) + ((uintn<4>)a << 24);
	return output;
}

constexpr inline uintn<8> fromint1(uintn<1> a, uintn<1> b, uintn<1> c, uintn<1> d, uintn<1> e, uintn<1> f, uintn<1> g, uintn<1> h){
	uintn<8> output = h + ((uintn<8>)g << 8) + ((uintn<8>)f << 16) + ((uintn<8>)e << 24) + ((uintn<8>)d << 32) + ((uintn<8>)c << 40) + ((uintn<8>)b << 48) + ((uintn<8>)a << 56);
	return output;
}

template<unsigned int N>
struct intn_t{
	typedef void type;
};

template<>
struct intn_t<1>{
	typedef int8_t type;
};
template<>
struct intn_t<2>{
	typedef int16_t type;
};
template<>
struct intn_t<4>{
	typedef int32_t type;
};
template<>
struct intn_t<8>{
	typedef int64_t type;
};

template<unsigned int N> using intn = typename intn_t<N>::type;

constexpr unsigned int const_clamp(unsigned int x, unsigned int low, unsigned int high){
	if(x < low)
		return low;
	if(x > high)
		return high;
	return x;
}

constexpr unsigned int const_max(unsigned int a, unsigned int b){
	if(b > a)
		return b;
	return a;
}
constexpr unsigned int const_min(unsigned int a, unsigned int b){
	if(b < a)
		return b;
	return a;
}

constexpr unsigned int fastestWordSize = const_clamp(sizeof(void*) / 2, 1, 4);

constexpr unsigned int divOrZero(unsigned int a, unsigned int b){
	if(a % b != 0)
		return 0;
	return a/b;
}

#endif
