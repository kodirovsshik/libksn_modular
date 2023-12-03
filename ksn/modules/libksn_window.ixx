
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
	window_t(window_t&&) = default;

	window_t& operator=(window_t&&) noexcept;

	window_t(const window_t&) = delete;
	window_t& operator=(const window_t&) = delete;



	template<class char_t = char>
	window_open_result_t open(u32 width, u32 height, const char_t* title = "", window_style_t style = window_style::default_style) noexcept;

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
	u32 get_current_monitor_refresh_rate() const noexcept;

	//Updates internal time counter and waits for a frame end according to set framerate
	//Is a no-op if no frame rate limit is set
	//Waiting strategy is unspecified
	void framerate_sync() noexcept;

	void framerate_sync_sleep() noexcept;
	void framerate_sync_busy() noexcept;
	void framerate_sync_hybrid() noexcept;


	u32 get_client_width() const noexcept;
	u32 get_client_height() const noexcept;
	std::pair<u32, u32> get_client_size() const noexcept;

	bool set_client_width(u32) noexcept;
	bool set_client_height(u32) noexcept;
	bool set_client_size(u32 width, u32 height) noexcept;
	bool set_client_size(std::pair<u32, u32>) noexcept;


	i32 get_client_x() const noexcept;
	i32 get_client_y() const noexcept;
	std::pair<i32, i32> get_client_position() const noexcept;

	bool set_client_x(i32) noexcept;
	bool set_client_y(i32) noexcept;
	bool set_client_position(i32 x, i32 y) noexcept;
	bool set_client_position(std::pair<i32, i32>) noexcept;


	//Tries to center the window
	void reset_window_position() noexcept;
	void set_fullscreen_windowed() noexcept;

	void set_cursor_visible(bool visible = true) noexcept;
	void set_cursor_captured(bool captured = true) noexcept;

	void hide() const noexcept;
	void show() const noexcept;

	template<class char_t>
	bool set_title(const char_t* name) const noexcept;

	bool set_size_constraints(u32 min_width, u32 min_height, u32 max_width, u32 max_height) noexcept;
	bool set_size_constraints(std::pair<u32, u32> min_size, std::pair<u32, u32> max_size) noexcept;

	bool set_min_width(u32) noexcept;
	bool set_max_width(u32) noexcept;
	bool set_min_height(u32) noexcept;
	bool set_max_height(u32) noexcept;

	void set_modifier_keys_check_on_keyboard_event(bool check_enabled = true) noexcept;

private:
	window_common_impl m_common_impl;
	window_impl m_impl;
};



_KSN_EXPORT_END
