
export module libksn.window:common_declaration;

import libksn.time;

import <ksn/ksn.hpp>;



_KSN_EXPORT_BEGIN

class window_common_impl
{
	ksn::stopwatch m_sw;
	ksn::duration period;

	window_common_impl& operator=(const window_common_impl&) = default;
	
public:
	void tick(void(*sleep_function)(duration)) noexcept;

	window_common_impl() = default;
	window_common_impl(window_common_impl&&);
};

_KSN_EXPORT_END
