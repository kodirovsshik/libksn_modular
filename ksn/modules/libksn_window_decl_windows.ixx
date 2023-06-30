
#pragma warning(disable : 4005 5105 5106)



export module libksn.window:os_specific_declaration;

import <ksn/ksn.hpp>;
import <Windows.h>;
import :auxillary;




_KSN_EXPORT_BEGIN

class window_t;
struct window_impl
{
	std::deque<event_t> m_queue;
	std::mutex m_queue_mutex;

	HWND m_window = nullptr;

	window_t* m_ksn_window = nullptr;

	window_resizemove_handle_t m_resizemove_handle = nullptr;

	std::pair<uint32_t, uint32_t> m_last_size{};
	std::pair<uint32_t, uint32_t> m_resizemove_last_size{};

	std::pair<int32_t, int32_t> m_last_pos{};
	std::pair<int32_t, int32_t> m_resizemove_last_pos{};

	std::pair<uint32_t, uint32_t> m_size_min{};
	std::pair<uint32_t, uint32_t> m_size_max = { UINT32_MAX, UINT32_MAX };

	wchar_t m_pending_wchar = 0;

	union
	{
		struct
		{
			bool m_is_repetitive_keyboard_enabled : 1;
			bool m_is_resizemove : 1; //Whether WM_ENTERSIZEMOVE was received but WM_EXITSIZEMOVE hasn't been yet
			bool m_check_special_keys_on_keyboard_event : 1;
			bool m_mouse_inside : 1;
			bool m_is_closing : 1;
			bool m_is_clipping : 1;
			bool m_is_thread_safe_events : 1;
			mutable bool m_filled_on_init : 1;
		};
		uint32_t m_flags;
	};



	void check_initial_blackout() const noexcept;

	void ensure_size_constraints() noexcept;

	window_impl* operator->() noexcept;
	const window_impl* operator->() const noexcept;
};

_KSN_EXPORT_END
