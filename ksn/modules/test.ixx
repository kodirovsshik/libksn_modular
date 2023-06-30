
export module test;

import <ksn/ksn.hpp>;



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

/*

//Code for quick testing if it was, by chance, finally implemented
@microsoft deducing this in modules when

class A
{
public:
	int foo(this auto&& self)
	{
		return 4;
	}
};
*/

_KSN_EXPORT_END





_KSN_BEGIN

_KSN_END
