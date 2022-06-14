
export module libksn.window:common_implementation;

import libksn.window;
import :common_declaration;
import libksn.time;

import <ksn/ksn.hpp>;



_KSN_EXPORT_BEGIN

void window_common_impl::tick(void(*sleeper)(ksn::time)) noexcept
{
	if ((uint64_t)this->period != 0)
	{
		time dt = this->m_sw.stop();
		sleeper(this->period - dt);
		this->m_sw.start();
	}
}



window_t::~window_t() noexcept
{
}

void window_t::framerate_sync() noexcept
{
	this->m_common_impl.tick(ksn::sleep_for);
}
void window_t::framerate_sync_hybrid() noexcept
{
	this->m_common_impl.tick(ksn::hybrid_sleep_for);
}

_KSN_EXPORT_END
