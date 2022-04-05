
export module test;

import <ksn/ksn.hpp>;



_KSN_EXPORT_BEGIN

constexpr int foo(int x);

_KSN_EXPORT_END





_KSN_BEGIN

constexpr int foo(int x)
{
	return 42;
}

_KSN_END
