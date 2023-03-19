
export module test;

import <ksn/ksn.hpp>;
import <concepts>;



_KSN_EXPORT_BEGIN

template<class T>
constexpr int foo(T x) { return 0; };

template<std::integral T>
constexpr int foo(T x)
{
	return 1;
}
template<std::floating_point T>
constexpr int foo(T x)
{
	return 2;
}

_KSN_EXPORT_END





_KSN_BEGIN

_KSN_END
