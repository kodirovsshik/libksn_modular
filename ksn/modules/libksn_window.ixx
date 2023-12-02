
export module libksn.window;

export import :auxillary;
import :os_specific_declaration;
import :common_declaration;

import <ksn/ksn.hpp>;



_KSN_EXPORT_BEGIN



struct event_t;

class window_t
{
public:
	virtual ~window_t() noexcept;

	window_t() noexcept;
	window_t(window_t&&) noexcept;

	window_t& operator=(window_t&&) noexcept;

	window_t(const window_t&) = delete;
	window_t& operator=(const window_t&) = delete;



	template<class char_t>
	window_open_result_t open(uint32_t width, uint32_t height, const char_t* title = L"", window_style_t style = window_style::default_style) noexcept;

	void close() noexcept;



	bool poll_event(event_t&) noexcept;
	bool wait_for_event(event_t&) noexcept;

	bool is_open() const noexcept;
	//Same as is_open()
	explicit operator bool() const noexcept;

	bool has_focus() const noexcept;
	void request_focus() const noexcept;

	void clear_framerate_limit() noexcept;
	void set_framerate_limit(float fps) noexcept;
	//returns INFINITY if no limit is set
	float get_framerate_limit() const noexcept;

	//Refresh rate of the monitor the OS considers window to be on
	uint32_t get_current_monitor_refresh_rate() const noexcept;

	//Updates internal time counter and waits for a frame end according to set framerate
	//Is a no-op if no frame rate limit is set
	//Waiting strategy is unspecified
	void framerate_sync() noexcept;

	void framerate_sync_sleep() noexcept;
	void framerate_sync_busy() noexcept;
	void framerate_sync_hybrid() noexcept;


	uint32_t get_client_width() const noexcept;
	uint32_t get_client_height() const noexcept;
	std::pair<uint32_t, uint32_t> get_client_size() const noexcept;

	bool set_client_width(uint32_t) noexcept;
	bool set_client_height(uint32_t) noexcept;
	bool set_client_size(uint32_t width, uint32_t height) noexcept;
	bool set_client_size(std::pair<uint32_t, uint32_t>) noexcept;


	int32_t get_client_x() const noexcept;
	int32_t get_client_y() const noexcept;
	std::pair<int32_t, int32_t> get_client_position() const noexcept;

	bool set_client_x(int32_t) noexcept;
	bool set_client_y(int32_t) noexcept;
	bool set_client_position(int32_t x, int32_t y) noexcept;
	bool set_client_position(std::pair<int32_t, int32_t>) noexcept;


	//Tries to center the window
	void reset_window_position() noexcept;
	void set_fullscreen_windowed() noexcept;

	void set_cursor_visible(bool visible = true) noexcept;
	void set_cursor_captured(bool captured = true) noexcept;

	void hide() const noexcept;
	void show() const noexcept;

	template<class char_t>
	bool set_title(const char_t* name) const noexcept;

	bool set_size_constraints(uint32_t min_width, uint32_t min_height, uint32_t max_width, uint32_t max_height) noexcept;
	bool set_size_constraints(std::pair<uint32_t, uint32_t> min_size, std::pair<uint32_t, uint32_t> max_size) noexcept;

	bool set_size_min_width(uint32_t) noexcept;
	bool set_size_max_width(uint32_t) noexcept;
	bool set_size_min_height(uint32_t) noexcept;
	bool set_size_max_height(uint32_t) noexcept;

	void set_modifier_keys_check_on_keyboard_event(bool check_enabled = true) noexcept;

private:
	window_common_impl m_common_impl;
	window_impl m_impl;
};



void swap(window_t& a, window_t& b) noexcept;

_KSN_EXPORT_END
