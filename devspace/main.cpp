
import libksn.math;
import libksn.debug;
import libksn.type_traits;

import std;



struct xint;

enum class xint_binop_enum
{
	add, mul
};

template<bool ref1, bool ref2, xint_binop_enum op>
struct xint_binop_promise;

struct xint
{
	ksn::instance_debugger obj;

	xint(int x) : obj(x) {}

	static void add(xint& dst, const xint& a, const xint& b)
	{
		dst.obj.payload = a.obj.payload + b.obj.payload;
	}
	static void mul(xint& dst, const xint& a, const xint& b)
	{
		dst.obj.payload = a.obj.payload * b.obj.payload;
	}

	template<bool ref1, bool ref2, xint_binop_enum op>
	xint& operator=(const xint_binop_promise<ref1, ref2, op>& promise);
};

template<bool _ref1, bool _ref2, xint_binop_enum _op>
struct xint_binop_promise
{
	static constexpr bool ref1 = _ref1;
	static constexpr bool ref2 = _ref2;
	static constexpr xint_binop_enum op = _op;

	template<bool test>
	using ref_or_val_t = std::conditional_t<test, const xint&, xint>;

	ref_or_val_t<ref1> x1;
	ref_or_val_t<ref2> x2;

	operator xint() const;

	void evaluate_into(xint& result) const
	{
		switch (op)
		{
		case xint_binop_enum::add: return xint::add(result, x1, x2);
		case xint_binop_enum::mul: return xint::mul(result, x1, x2);
		}
	}
};

template<class T>
struct is_some_xint_binop_promise : std::false_type {};
template<bool ref1, bool ref2, xint_binop_enum op>
struct is_some_xint_binop_promise<xint_binop_promise<ref1, ref2, op>> : std::true_type {};

template<class T>
concept xint_arithmetic_argument = ksn::universal_reference<T, xint> || is_some_xint_binop_promise<std::remove_cvref_t<T>>::value;

template<class T>
static constexpr bool should_save_as_value = false;

template<xint_arithmetic_argument A, xint_arithmetic_argument B>
auto operator+(A&& a, B&& b)
{
	return xint_binop_promise<
		should_save_as_value<A>, should_save_as_value<B>, xint_binop_enum::add
	>{ std::forward<A>(a), std::forward<B>(b) };
}



int main()
{
	xint a(1), b(2);
	auto r1 = a + b;
	is_some_xint_binop_promise<decltype(r1)>::value;
	auto r2 = r1 + r1;

	[] {}();
 }

template<bool ref1, bool ref2, xint_binop_enum op>
inline xint_binop_promise<ref1, ref2, op>::operator xint() const
{
	xint result;
	this->evaluate_into(result);
	return result;
}

template<bool ref1, bool ref2, xint_binop_enum op>
xint& xint::operator=(const xint_binop_promise<ref1, ref2, op>& promise)
{
	promise.evaluate_into(*this);
	return *this;
}
