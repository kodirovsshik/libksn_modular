
export module libksn.math:common;

import libksn.type_traits;

import <ksn/ksn.hpp>;
import <iterator>;
import <cmath>;


_KSN_EXPORT_BEGIN


//TODO:
// polynomial_multiplication
// polynomial_interpolation
// polynomial_roots
// figure out constexpr


template<class T>
constexpr T sign(const T& x);
template<std::floating_point T>
constexpr T sign(T x);

template<class fp_t>
constexpr fp_t map(const fp_t& x, const fp_t& from_begin, const fp_t& from_end, const fp_t& to_begin, const fp_t& to_end);

_KSN_EXPORT_END




_KSN_BEGIN


template<class T>
constexpr T sign(const T& x)
{
	if (x != x)
		return x;
	T c = 0;
	if (x)
		c = 1;
	T a = -1;
	if (x >= 0)
		a = c;
	return a;
}
template<std::floating_point T>
constexpr T sign(T x)
{
	if (x != x)
		return x;
	T vsign = std::copysign(1, x);
	vsign *= (x != 0);
	return vsign;
}


_KSN_END
