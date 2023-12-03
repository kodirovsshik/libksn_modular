
#pragma warning(disable : 4005 5105 5106)

export module libksn.window:os_specific_implementation;

import :auxillary;
import :os_specific_declaration;
import libksn.window;

import <ksn/ksn.hpp>;
import <Windows.h>;
#undef min
#undef max



_KSN_BEGIN

window_t::window_t() noexcept
{
	this->m_impl->m_ksn_window = this;
}



std::pair<u32, u32> window_t::get_client_size() const noexcept
{
	return this->m_impl->m_last_size;
}
std::pair<i32, i32> window_t::get_client_position() const noexcept
{
	return this->m_impl->m_last_pos;
}
bool window_t::set_client_size(u32 width, u32 height) noexcept
{
	//TODO
	return false;
}
bool window_t::set_client_position(i32 x, i32 y) noexcept
{
	//TODO
	return false;
}




bool _process_pfd(HDC hdc)
{
	//Do boring repetitive WINAPI stuff 
	PIXELFORMATDESCRIPTOR pfd{};
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;

	return SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);
}

void _process_msg(MSG& msg)
{
	TranslateMessage(&msg);
	DispatchMessageW(&msg);
}

keyboard_button_t _get_button(WPARAM key, LPARAM flags)
{
	using kb = keyboard_button_t;
	switch (key)
	{
	case VK_SHIFT:
	{
		static const UINT lshift_scancode = MapVirtualKeyW(VK_LSHIFT, MAPVK_VK_TO_VSC);
		UINT current_scancode = (flags >> 16) & 0xFF;
		return current_scancode == lshift_scancode ? kb::shift_left : kb::shift_right;
	}
	case VK_MENU: return (HIWORD(flags) & KF_EXTENDED) ? kb::alt_right : kb::alt_left;
	case VK_CONTROL: return (HIWORD(flags) & KF_EXTENDED) ? kb::ctrl_right : kb::ctrl_left;
	case VK_LWIN: return kb::system_left;
	case VK_RWIN: return kb::system_right;
	case VK_APPS: return (HIWORD(flags) & KF_EXTENDED) ? kb::menu_right : kb::menu_left;
	case VK_OEM_1: return kb::semicolon;
	case VK_OEM_PLUS: return kb::equal_sign;
	case VK_OEM_COMMA: return kb::comma;
	case VK_OEM_MINUS: return kb::minus;
	case VK_OEM_PERIOD: return kb::period;
	case VK_OEM_2: return kb::slash;
	case VK_OEM_3: return kb::tilde;
	case VK_OEM_4: return kb::bracket_left;
	case VK_OEM_6: return kb::bracket_right;
	case VK_OEM_7: return kb::quote;
	case VK_OEM_5: return kb::backslash;
	case VK_ESCAPE: return kb::esc;
	case VK_SPACE: return kb::space;
	case VK_RETURN: return kb::enter;
	case VK_BACK: return kb::backspace;
	case VK_TAB: return kb::tab;
	case VK_PRIOR: return kb::page_up;
	case VK_NEXT: return kb::page_down;
	case VK_END: return kb::end;
	case VK_HOME: return kb::home;
	case VK_INSERT: return kb::insert;
	case VK_DELETE: return kb::delete_;
	case VK_ADD: return kb::add;
	case VK_SUBTRACT: return kb::substract;
	case VK_MULTIPLY: return kb::multiply;
	case VK_DIVIDE: return kb::divide;
	case VK_PAUSE: return kb::pause;
	case VK_F1: return kb::F1;
	case VK_F2: return kb::F2;
	case VK_F3: return kb::F3;
	case VK_F4: return kb::F4;
	case VK_F5: return kb::F5;
	case VK_F6: return kb::F6;
	case VK_F7: return kb::F7;
	case VK_F8: return kb::F8;
	case VK_F9: return kb::F9;
	case VK_F10: return kb::F10;
	case VK_F11: return kb::F11;
	case VK_F12: return kb::F12;
	case VK_F13: return kb::F13;
	case VK_F14: return kb::F14;
	case VK_F15: return kb::F15;
	case VK_F16: return kb::F16;
	case VK_F17: return kb::F17;
	case VK_F18: return kb::F18;
	case VK_F19: return kb::F19;
	case VK_F20: return kb::F20;
	case VK_F21: return kb::F21;
	case VK_F22: return kb::F22;
	case VK_F23: return kb::F23;
	case VK_F24: return kb::F24;
	case VK_LEFT: return kb::arrow_left;
	case VK_RIGHT: return kb::arrow_right;
	case VK_UP: return kb::arrow_up;
	case VK_DOWN: return kb::arrow_down;

	case VK_NUMPAD0:
	case VK_NUMPAD1:
	case VK_NUMPAD2:
	case VK_NUMPAD3:
	case VK_NUMPAD4:
	case VK_NUMPAD5:
	case VK_NUMPAD6:
	case VK_NUMPAD7:
	case VK_NUMPAD8:
	case VK_NUMPAD9:
		return keyboard_button_t((int)kb::numpad0 + (int)key - VK_NUMPAD0);

	case VK_NUMLOCK: return kb::num_lock;
	case VK_CAPITAL: return kb::caps_lock;
	case VK_SCROLL: return kb::scroll_lock;

	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
		return keyboard_button_t((int)kb::a + (int)key - 'A');

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return keyboard_button_t((int)kb::digit0 + (int)key - '0');

	default:
		return kb::other;
	}
}

BOOL AdjustWindowRectInverse(LPRECT rect, DWORD dwStyle)
{
	RECT rc;
	SetRectEmpty(&rc);
	const bool ok = AdjustWindowRect(&rc, dwStyle, false);
	if (ok)
	{
		rect->left -= rc.left;
		rect->right -= rc.right;
		rect->top -= rc.top;
		rect->bottom -= rc.bottom;
	}
	return ok;
}

std::pair<u32, u32> adjust_size_to_client(int width, int height, HWND window)
{
	RECT rc{};
	rc.right = width;
	rc.bottom = height;
	AdjustWindowRectInverse(&rc, GetWindowLong(window, GWL_STYLE));
	return std::pair<u32, u32>
	{
		(u32)std::min((i64)rc.right - rc.left, (i64)UINT32_MAX),
		(u32)std::min((i64)rc.bottom - rc.top, (i64)UINT32_MAX)
	};
}

struct minmax_info
{
	u32 width_min;
	u32 width_max;
	u32 height_min;
	u32 height_max;
};
minmax_info GetSystemMetricsClient(HWND wnd)
{
	minmax_info info;

	RECT rcmin{};
	rcmin.right = GetSystemMetrics(SM_CXMINTRACK);
	rcmin.bottom = GetSystemMetrics(SM_CYMINTRACK);

	RECT rcmax{};
	rcmax.right = GetSystemMetrics(SM_CXMAXTRACK);
	rcmax.bottom = GetSystemMetrics(SM_CYMAXTRACK);

	DWORD style = GetWindowLongA(wnd, GWL_STYLE);

	AdjustWindowRectInverse(&rcmin, style);
	AdjustWindowRectInverse(&rcmax, style);

	info.width_min = (u32)std::min((i64)rcmin.right - rcmin.left, (i64)UINT32_MAX);
	info.width_max = (u32)std::min((i64)rcmax.right - rcmax.left, (i64)UINT32_MAX);
	info.height_min = (u32)std::min((i64)rcmin.bottom - rcmin.top, (i64)UINT32_MAX);
	info.height_max = (u32)std::min((i64)rcmax.bottom - rcmax.top, (i64)UINT32_MAX);

	return info;
}

LRESULT WINAPI libksn_window_procedure(HWND wnd, UINT msg, WPARAM w, LPARAM l)
{
#define lock_event_queue(pimpl) \
		auto __event_queue_lock = (pimpl)->m_is_thread_safe_events ? \
			std::unique_lock<std::mutex>((pimpl)->m_queue_mutex) \
			: std::unique_lock<std::mutex>();


	msg = LOWORD(msg);



	if (msg == WM_CREATE)
	{
		ksn::window_impl& win_impl = *(window_impl*)(((CREATESTRUCTW*)(l))->lpCreateParams);
		std::deque<event_t>& q = win_impl.m_queue;

		lock_event_queue(&win_impl);

		SetWindowLongPtrW(wnd, GWLP_USERDATA, (LONG_PTR)&win_impl);

		event_t ev;
		ev.type = event_type_t::create;
		q.push_back(ev);

		return 0;
	}

	else if (msg == WM_GETMINMAXINFO)
	{
		MINMAXINFO* info = (MINMAXINFO*)l;

		ksn::window_impl* pimpl = (window_impl*)GetWindowLongPtrW(wnd, GWLP_USERDATA);
		if (!pimpl)
		{
			RECT rc{};
			rc.right = rc.bottom = 65535;

			AdjustWindowRect(&rc, GetWindowLongA(wnd, GWL_STYLE), false);
			info->ptMaxTrackSize.x = rc.right - rc.left;
			info->ptMaxTrackSize.y = rc.bottom - rc.top;
			return 0;
		}

		ksn::window_impl& win_impl = *pimpl;

		DWORD style = GetWindowLongA(win_impl.m_window, GWL_STYLE);

		RECT rcmin{}, rcmax{};
		rcmin.right = win_impl.m_size_min.first;
		rcmin.bottom = win_impl.m_size_min.second;
		rcmax.right = win_impl.m_size_max.first;
		rcmax.bottom = win_impl.m_size_max.second;

		AdjustWindowRect(&rcmin, style, false);
		AdjustWindowRect(&rcmax, style, false);

		info->ptMinTrackSize.x = rcmin.right - rcmin.left;;
		info->ptMinTrackSize.y = rcmin.bottom - rcmin.top;
		info->ptMaxTrackSize.x = rcmax.right - rcmax.left;
		info->ptMaxTrackSize.y = rcmax.bottom - rcmax.top;
		return 0;
	}


	ksn::window_impl& win_impl = *(window_impl*)GetWindowLongPtrW(wnd, GWLP_USERDATA);
	std::deque<event_t>& q = win_impl.m_queue;


#define push_mouse_event(_button, is_pressed) \
		{\
			event_t ev; \
			ev.type = is_pressed ? event_type_t::mouse_press : event_type_t::mouse_release; \
			ev.mouse_button_data.button = mouse_button_t::_button; \
			ev.mouse_button_data.x = (u32)LOWORD(l); \
			ev.mouse_button_data.y = (u32)HIWORD(l); \
			lock_event_queue(&win_impl); \
			q.push_back(ev); \
		}\



#define push_mouse_scroll_event(vertical) \
		{ \
			POINT pos{ (LONG)LOWORD(l), (LONG)HIWORD(l) }; \
			ScreenToClient(win_impl.m_window, &pos); \
 \
			event_t ev; \
			ev.type = vertical ? event_type_t::mouse_scroll_vertical : event_type_t::mouse_scroll_horizontal; \
			ev.mouse_scroll_data.is_vertical = (msg == WM_MOUSEWHEEL); \
			ev.mouse_scroll_data.delta = (float)(int16_t)HIWORD(w) / 120; \
			ev.mouse_scroll_data.x = (u32)pos.x; \
			ev.mouse_scroll_data.y = (u32)pos.y; \
 \
			lock_event_queue(&win_impl);\
			q.push_back(ev); \
		}


	switch (msg)
	{
	case WM_CLOSE:
	{
		event_t ev;
		ev.type = event_type_t::close;

		lock_event_queue(&win_impl);
		q.push_back(ev);
		return 0;
	}
	break;

	case WM_SIZE:
	{
		if (w == SIZE_MAXIMIZED)
		{
			event_t ev;
			ev.type = event_type_t::maximized;
			lock_event_queue(&win_impl);
			q.push_back(ev);
		}

		if (w == SIZE_MINIMIZED)
		{
			event_t ev;
			ev.type = event_type_t::minimized;
			lock_event_queue(&win_impl);
			q.push_back(ev);
		}
		else if (w != SIZE_MAXHIDE && w != SIZE_MAXSHOW)
		{
			resizemove_data_t data;
			data.window = win_impl.m_ksn_window;

			auto new_size = win_impl.m_ksn_window->get_client_size();

			if (new_size == win_impl.m_resizemove_last_size)
				break;

			data.resize = true;
			data.window_resize_data.width_old = win_impl.m_resizemove_last_size.first;
			data.window_resize_data.height_old = win_impl.m_resizemove_last_size.second;
			data.window_resize_data.width_new = new_size.first;
			data.window_resize_data.height_new = new_size.second;

			//DeleteObject(win_impl.m_bitmap);
			//win_impl.m_bitmap = CreateCompatibleBitmap(win_impl.m_hdc, new_size.first, new_size.second);
			//SelectObject(win_impl.m_hmdc, win_impl.m_bitmap);

			win_impl.m_resizemove_last_size = new_size;
			if (win_impl.m_resizemove_handle)
				win_impl.m_resizemove_handle(&data);
			else
			{
				event_t ev;
				ev.type = event_type_t::resize;

				ev.window_resize_data.height_new = data.window_resize_data.height_new;
				ev.window_resize_data.height_old = data.window_resize_data.height_old;
				ev.window_resize_data.width_new = data.window_resize_data.width_new;
				ev.window_resize_data.width_old = data.window_resize_data.width_old;

				lock_event_queue(&win_impl);
				q.push_back(ev);
			}
		}
	}
	break;

	case WM_MOVE:
	{
		resizemove_data_t data;
		data.window = win_impl.m_ksn_window;

		auto new_pos = win_impl.m_ksn_window->get_client_position();

		data.move = true;
		data.window_move_data.x_old = win_impl.m_resizemove_last_pos.first;
		data.window_move_data.y_old = win_impl.m_resizemove_last_pos.second;
		data.window_resize_data.width_new = new_pos.first;
		data.window_resize_data.height_new = new_pos.second;

		win_impl.m_resizemove_last_pos = new_pos;
		if (win_impl.m_resizemove_handle)
			win_impl.m_resizemove_handle(&data);
		else
		{
			event_t ev;
			ev.type = event_type_t::move;

			ev.window_move_data.x_new = data.window_move_data.x_new;
			ev.window_move_data.x_old = data.window_move_data.x_old;
			ev.window_move_data.y_new = data.window_move_data.y_new;
			ev.window_move_data.y_old = data.window_move_data.y_old;

			lock_event_queue(&win_impl);
			q.push_back(ev);
		}
	}
	break;

	case WM_ENTERSIZEMOVE:
	{
		ClipCursor(NULL);

		RECT client_area;
		GetClientRect(win_impl.m_window, &client_area);

		win_impl.m_last_pos.first = (i32)client_area.left;
		win_impl.m_last_pos.second = (i32)client_area.bottom;

		win_impl.m_last_size.first = (u32)(client_area.right - client_area.left);
		win_impl.m_last_size.second = (u32)(client_area.bottom - client_area.top);

		win_impl.m_is_resizemove = true;
	}
	break;

	case WM_EXITSIZEMOVE:
	{
		WINDOWINFO info;
		info.cbSize = sizeof(info);
		GetWindowInfo(win_impl.m_window, &info);
		RECT client_area = info.rcClient;;

		if (win_impl.m_is_clipping)
			ClipCursor(&client_area);

		u32 width = u32(client_area.right - client_area.left);
		u32 height = u32(client_area.bottom - client_area.top);

		auto new_size = std::pair<u32, u32>(width, height);
		auto new_pos = std::pair<i32, i32>((i32)client_area.left, (i32)client_area.top);

		win_impl.m_is_resizemove = false;

		if (new_pos != win_impl.m_last_pos)
		{
			event_t ev;
			ev.type = event_type_t::move;

			ev.window_move_data.x_new = new_pos.first;
			ev.window_move_data.y_new = new_pos.second;

			ev.window_move_data.x_old = win_impl.m_last_pos.first;
			ev.window_move_data.y_old = win_impl.m_last_pos.second;

			lock_event_queue(&win_impl);
			q.push_back(ev);
		}
		if (new_size != win_impl.m_last_size)
		{
			//DeleteObject(win_impl.m_bitmap);
			//win_impl.m_bitmap = CreateCompatibleBitmap(win_impl.m_hdc, new_size.first, new_size.second);
			//SelectObject(win_impl.m_hmdc, win_impl.m_bitmap);

			event_t ev;
			ev.type = event_type_t::resize;

			ev.window_resize_data.width_new = new_size.first;
			ev.window_resize_data.height_new = new_size.second;

			ev.window_resize_data.width_old = win_impl.m_last_size.first;
			ev.window_resize_data.height_old = win_impl.m_last_size.second;

			lock_event_queue(&win_impl);
			q.push_back(ev);
		}
	}
	break;

	case WM_SETFOCUS:
	{
		if (win_impl.m_is_clipping)
		{
			WINDOWINFO info;
			info.cbSize = sizeof(info);
			GetWindowInfo(win_impl.m_window, &info);
			ClipCursor(&info.rcClient);
		}

		event_t ev;
		ev.type = event_type_t::focus_gained;
		lock_event_queue(&win_impl);
		q.push_back(ev);
	}
	break;

	case WM_KILLFOCUS:
	{
		ClipCursor(NULL);

		if (!win_impl.m_is_closing)
		{
			event_t ev;
			ev.type = event_type_t::focus_lost;
			lock_event_queue(&win_impl);
			q.push_back(ev);
		}
	}
	break;

	case WM_CHAR:
	{
		if (win_impl.m_is_repetitive_keyboard_enabled || (l & 0xFFFF) == 0)
		{
			wchar_t ch = (wchar_t)w;

			uint8_t high6 = ch >> 10; //6 bits of header
			wchar_t high_ch = win_impl.m_pending_wchar; //previous UTF-16 part of character (if present)

			if (high6 == 0b110110)
			{
				win_impl.m_pending_wchar = ch;
			}
			else if (high6 == 0b110111 && high_ch)
			{
				event_t ev;
				ev.type = event_type_t::text;
				ev.character = ((high_ch & 0b0000001111111111) << 16) | (ch & 0b0000001111111111);

				lock_event_queue(&win_impl);
				q.push_back(ev);

				win_impl.m_pending_wchar = 0;
			}
			else //Single UTF-16 unit
			{
				event_t ev;
				ev.type = event_type_t::text;
				ev.character = (u32)w;

				lock_event_queue(&win_impl);
				q.push_back(ev);
			}
		}
	}
	break;

	case WM_UNICHAR:
	{
		if (w == UNICODE_NOCHAR) return FALSE; //As microsoft intended

		if (win_impl.m_is_repetitive_keyboard_enabled || (l & 0xFFFF) == 0)
		{
			event_t ev;
			ev.type = event_type_t::text;
			ev.character = (u32)w;
		}
	}
	break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		bool press = msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN;
		bool repeated = HIWORD(l) & KF_REPEAT;
		if (!press || win_impl.m_is_repetitive_keyboard_enabled || !repeated)
		{
			event_t ev;
			if (press)
			{
				if (repeated)
					ev.type = event_type_t::keyboard_hold;
				else
					ev.type = event_type_t::keyboard_press;
			}
			else
				ev.type = event_type_t::keyboard_release;

			ev.keyboard_button_data.button = _get_button(w, l);

			if (win_impl.m_check_special_keys_on_keyboard_event)
			{
				//TODO: test this code
				ev.keyboard_button_data.alt = HIWORD(GetKeyState(VK_MENU)) != 0;
				ev.keyboard_button_data.control = HIWORD(GetKeyState(VK_CONTROL)) != 0;
				ev.keyboard_button_data.shift = HIWORD(GetKeyState(VK_SHIFT)) != 0;
				ev.keyboard_button_data.system = HIWORD(GetKeyState(VK_LWIN)) || HIWORD(GetKeyState(VK_RWIN));
			}

			lock_event_queue(&win_impl);
			q.push_back(ev);
		}
	}
	break;

	case WM_MOUSEWHEEL:
		push_mouse_scroll_event(true);
		break;

	case WM_MOUSEHWHEEL:
		push_mouse_scroll_event(false);
		break;

	case WM_LBUTTONDOWN:
		push_mouse_event(left, true);
		break;

	case WM_LBUTTONUP:
		push_mouse_event(left, false);
		break;

	case WM_RBUTTONDOWN:
		push_mouse_event(right, true);
		break;

	case WM_RBUTTONUP:
		push_mouse_event(right, false);
		break;

	case WM_MBUTTONDOWN:
		push_mouse_event(middle, true);
		break;

	case WM_MBUTTONUP:
		push_mouse_event(middle, false);
		break;

	case WM_XBUTTONUP:
		if (HIWORD(w) == XBUTTON1)
			push_mouse_event(extra1, true)
		else
			push_mouse_event(extra2, true);
		break;

	case WM_XBUTTONDOWN:
		if (HIWORD(w) == XBUTTON1)
			push_mouse_event(extra1, false)
		else
			push_mouse_event(extra2, false);
		break;

	case WM_MOUSELEAVE:
	{
		if (win_impl.m_mouse_inside)
		{
			win_impl.m_mouse_inside = false;

			ksn::event_t ev;
			ev.type = event_type_t::mouse_leave;

			lock_event_queue(&win_impl);
			q.push_back(ev);
		}
	}
	break;

	case WM_MOUSEMOVE:
	{
		event_t ev;

		if (win_impl.m_mouse_inside == false)
		{
			ev.type = event_type_t::mouse_entered;
			{
				lock_event_queue(&win_impl);
				q.push_back(ev);
			}
			win_impl.m_mouse_inside = true;

			TRACKMOUSEEVENT track_info;
			track_info.cbSize = sizeof(track_info);
			track_info.dwFlags = TME_LEAVE;
			track_info.hwndTrack = win_impl.m_window;
			TrackMouseEvent(&track_info);
		}
		i32 x = (i32)LOWORD(l);
		i32 y = (i32)HIWORD(l);

		ev.type = event_type_t::mouse_move;
		ev.mouse_move_data.x = x;
		ev.mouse_move_data.y = y;

		lock_event_queue(&win_impl);
		q.push_back(ev);
	}
	break;

#undef push_mouse_event
#undef push_mouse_scroll_event
	//case WM_SHOWWINDOW:
	case WM_PAINT:
		win_impl.check_initial_blackout();
		break;

	default:
		break;
	};


	if ((msg == WM_SYSCOMMAND) && (w == SC_KEYMENU))
		return 0;

	return DefWindowProcW(wnd, msg, w, l);
}





void window_impl::check_initial_blackout() const noexcept
{
	if (!this->m_filled_on_init)
	{
		PAINTSTRUCT pm;
		BeginPaint(this->m_window, &pm);
		GetClientRect(this->m_window, &pm.rcPaint);

		HBRUSH black_brush = CreateSolidBrush(RGB(0, 0, 0));
		auto hdc = GetDC(this->m_window);

		FillRect(hdc, &pm.rcPaint, black_brush);

		ReleaseDC(this->m_window, hdc);
		DeleteObject(black_brush);

		EndPaint(this->m_window, &pm);
		this->m_filled_on_init = true;
	}
}

window_impl* window_impl::operator->() noexcept
{
	return this;
}
const window_impl* window_impl::operator->() const noexcept
{
	return this;
}

void window_impl::ensure_size_constraints() noexcept
{
	const auto size = this->m_ksn_window->get_client_size();

	std::pair<u32, u32> new_size =
	{
		std::clamp(size.first, this->m_size_min.first, this->m_size_max.first),
		std::clamp(size.second, this->m_size_min.second, this->m_size_max.second)
	};

	if (new_size == size)
		return; //No unnecessary system calls
	this->m_ksn_window->set_client_size(new_size);
}

_KSN_END
