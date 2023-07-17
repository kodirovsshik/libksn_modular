
export module libksn.compress:common;

import <ksn/ksn.hpp>;



_KSN_EXPORT_BEGIN

template<class T>
using in_span_t = std::span<std::add_const_t<typename T::in_type>>;
template<class T>
using out_span_t = std::span<typename T::out_type>;

template<typename T>
concept compressor = requires(T t, in_span_t<T> in_span)
{
	t.compress(in_span);
	t.finish();

	{ t.data() } -> std::same_as<out_span_t<T>>;
	t.flush();
	t.reset();
};

_KSN_EXPORT_END
