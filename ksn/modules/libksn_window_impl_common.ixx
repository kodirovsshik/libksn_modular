
export module libksn.window:common_implementation;

import <ksn/ksn.hpp>;
import libksn.window;
import :common_declaration;



_KSN_EXPORT_BEGIN

void window_common_impl::tick(void(*sleeper)(duration)) noexcept
{
	if (this->period.as_nsec() != 0)
	{
		duration dt = this->m_sw.stop();
		sleeper(this->period - dt);
		this->m_sw.start();
	}
}



uint32_t window_t::get_client_width() const noexcept
{
	return this->get_client_size().first;
}
uint32_t window_t::get_client_height() const noexcept
{
	return this->get_client_size().second;
}
bool window_t::set_client_width(uint32_t w) noexcept
{
	return this->set_client_size(w, this->get_client_height());
}
bool window_t::set_client_height(uint32_t h) noexcept
{
	return this->set_client_size(this->get_client_width(), h);
}
int32_t window_t::get_client_x() const noexcept
{
	return this->get_client_position().first;
}
int32_t window_t::get_client_y() const noexcept
{
	return this->get_client_position().second;
}
bool window_t::set_client_x(int32_t x) noexcept
{
	return this->set_client_position(x, this->get_client_y());
}
bool window_t::set_client_y(int32_t y) noexcept
{
	return this->set_client_position(this->get_client_x(), y);
}

bool window_t::set_client_position(int32_t x, int32_t y) noexcept
{
	//TODO
	return false;
}

bool window_t::set_client_position(std::pair<int32_t, int32_t> pos) noexcept
{
	return this->set_client_position(pos.first, pos.second);
}

bool window_t::set_client_size(std::pair<uint32_t, uint32_t> size) noexcept
{
	return this->set_client_size(size.first, size.second);
}



window_t::~window_t() noexcept
{
}

void window_t::framerate_sync() noexcept
{
	this->framerate_sync_sleep();
}
void window_t::framerate_sync_sleep() noexcept
{
	this->m_common_impl.tick(ksn::sleep_for);
}
void window_t::framerate_sync_busy() noexcept
{
	this->m_common_impl.tick(ksn::busy_sleep_for);
}
void window_t::framerate_sync_hybrid() noexcept
{
	this->m_common_impl.tick(ksn::hybrid_sleep_for);
}

_KSN_EXPORT_END
