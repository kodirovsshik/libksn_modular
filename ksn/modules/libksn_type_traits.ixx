
export module libksn.type_traits;

import <ksn/ksn.hpp>;
import <type_traits>;



_KSN_BEGIN

_KSN_DETAIL_BEGIN


template<size_t x, size_t c = 0>
struct ct_log2
{
	static constexpr size_t value = ct_log2<x / 2, c + 1>::value;
};
template<size_t c>
struct ct_log2<1, c>
{
	static constexpr size_t value = c;
};
template<size_t c>
struct ct_log2<0, c>
{
};





template<size_t...>
struct max_v_helper
{
};
template<size_t A, size_t B, size_t... C>
struct max_v_helper<A, B, C...>
{
	static constexpr size_t value = A > B ? max_v_helper<A, C...>::value : max_v_helper<B, C...>::value;
};
template<size_t A>
struct max_v_helper<A>
{
	static constexpr size_t value = A;
};

template<size_t... Args>
static constexpr size_t max_v = max_v_helper<Args...>::value;

_KSN_DETAIL_END

_KSN_END





_KSN_EXPORT_BEGIN

template<typename T>
concept arithmetic = std::is_arithmetic_v<T>;





template<class T, class P0, class ...P1toN>
struct is_any_of : is_any_of<T, P1toN...> {};

template<class T, class ...P1toN>
struct is_any_of<T, T, P1toN...> : std::true_type {};

template<class T>
struct is_any_of<T, T> : std::true_type {};

template<class T, class U>
struct is_any_of<T, U> : std::false_type {};

template<class T, class ...Args>
constexpr bool is_any_of_v = is_any_of<T, Args...>::value;

template<class T, class ...Args>
concept any_of = is_any_of_v<T, Args...>;





template<class A, class B>
struct is_same_to_cv
	: std::bool_constant< std::is_same_v<std::remove_cv_t<A>, std::remove_cv_t<B>> >
{
};

template<class A, class B>
constexpr bool is_same_to_cv_v = is_same_to_cv<A, B>::value;

template<class T, class U>
concept same_to_cv = is_same_to_cv_v<T, U>;





template<class WHAT, class TO>
concept universal_reference = ksn::is_same_to_cv_v<std::remove_reference_t<TO>, std::remove_reference_t<WHAT>>;





template<size_t N>
static constexpr size_t ct_log2 = detail::ct_log2<N>::value;





//From https://stackoverflow.com/questions/16337610/how-to-know-if-a-type-is-a-specialization-of-stdvector

template<typename Test, template<typename...> class Ref>
struct is_specialization : std::false_type {};

template<template<typename...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref> : std::true_type {};

template<typename Test, template<typename...> class Ref>
static constexpr bool is_specialization_v = is_specialization<Test, Ref>::value;

template<typename Test, template<typename...> class Ref>
concept specialization_of = is_specialization_v<Test, Ref>;





template<typename char_t>
concept character = is_any_of_v<char_t, char, signed char, unsigned char, wchar_t, char8_t, char16_t, char32_t>;





template<size_t Arg0, size_t... ArgN>
static constexpr size_t max_v = detail::max_v_helper<Arg0, ArgN...>::value;

_KSN_EXPORT_END
