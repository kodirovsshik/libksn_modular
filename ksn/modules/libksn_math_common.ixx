
export module libksn.math:common;

import libksn.type_traits;

import <ksn/ksn.hpp>;
import <iterator>;


_KSN_EXPORT_BEGIN


//TODO:
// polynomial_multiplication
// polynomial_interpolation
// polynomial_roots
// figure out constexpr



template<class T>
T sign(const T& x);

template<class fp_t>
fp_t map(const fp_t& x, const fp_t& from_begin, const fp_t& from_end, const fp_t& to_begin, const fp_t& to_end);


template<class fpx_t>
struct newton_method_params
{
	size_t cycles;
	fpx_t delta; //to be used for small steps
	double epsilon; //to be compared with certain abs() calls

	newton_method_params() noexcept;
};

template<class fpx_t, class callable_t, class... args_t>
constexpr bool newton_method(callable_t&& func, fpx_t& result, newton_method_params<fpx_t> params = {}, args_t&& ...callee_args);


_KSN_EXPORT_END




_KSN_EXPORT_BEGIN


template<class fpx_t, class callable_t, class... args_t>
constexpr bool newton_method(callable_t&& f, fpx_t& x, newton_method_params<fpx_t> params, args_t&& ...args)
{
	const auto& epsilon = params.epsilon;
	const auto& dx = params.delta;
	auto& cycles_left = params.cycles;


}

_KSN_EXPORT_END
