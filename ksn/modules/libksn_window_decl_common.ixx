
export module libksn.window:common_declaration;

import libksn.time;

import <ksn/ksn.hpp>;



_KSN_EXPORT_BEGIN

class window_common_impl
{
	ksn::stopwatch m_sw;
	ksn::duration period;

public:
	void tick(void(*sleep_function)(duration)) noexcept;
};

_KSN_EXPORT_END
