
export module test;

#define _KSN_EXPORT_BEGIN export namespace ksn {
#define _KSN_EXPORT_END }

#define _KSN_BEGIN namespace ksn{
#define _KSN_END }



_KSN_EXPORT_BEGIN

constexpr int foo(int x);

_KSN_EXPORT_END





_KSN_BEGIN

constexpr int foo(int x)
{
	return 42;
}

_KSN_END
