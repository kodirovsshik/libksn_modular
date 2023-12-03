
export module libksn.window:auxillary;

import <ksn/ksn.hpp>;



_KSN_EXPORT_BEGIN

enum class event_type_t : uint8_t
{
	create = 0,
	open = create,
	close,
	move,
	resize,
	mouse_press,
	mouse_release,
	mouse_scroll,
	mouse_scroll_vertical = mouse_scroll,
	mouse_scroll_horizontal,
	mouse_move,
	keyboard_press,
	keyboard_hold,
	keyboard_release,
	focus_gained,
	focus_lost,
	mouse_entered,
	mouse_leave,
	maximized,
	minimized,
	text,

	event_types_count
};



enum class mouse_button_t : uint8_t
{
	left = 0, right, middle, extra1, extra2,
	buttons_count
};

enum class keyboard_button_t : uint8_t
{
	unknown = uint8_t(-1),
	a = 0,
	b,
	c,
	d,
	e,
	f,
	g,
	h,
	i,
	j,
	k,
	l,
	m,
	n,
	o,
	p,
	q,
	r,
	s,
	t,
	u,
	v,
	w,
	x,
	y,
	z,

	shift_left,
	shift_right,

	ctrl_left,
	control_left = ctrl_left,
	ctrl_right,
	control_right = ctrl_right,

	alt_left,
	alt_right,

	system_left,
	system_right,

	//context menu key
	menu_left,
	//context menu key
	menu_right,

	enter,
	backspace,

	minus,
	hypen = minus,
	equal,
	equal_sign = equal,
	bracket_left,
	bracket_right,
	semicolon,
	quote,
	backslash,
	comma,
	period,
	slash,

	num_lock,
	caps_lock,
	scroll_lock,

	tab,
	esc,
	escape = esc,
	tilde,
	space,

	digit0,
	digit1,
	digit2,
	digit3,
	digit4,
	digit5,
	digit6,
	digit7,
	digit8,
	digit9,

	numpad0,
	numpad1,
	numpad2,
	numpad3,
	numpad4,
	numpad5,
	numpad6,
	numpad7,
	numpad8,
	numpad9,

	add,
	substract,
	multiply,
	divide,

	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	F13,
	F14,
	F15,
	F16,
	F17,
	F18,
	F19,
	F20,
	F21,
	F22,
	F23,
	F24,


	insert,
	delete_,
	home,
	end,
	page_up,
	page_down,

	arrow_up,
	arrow_down,
	arrow_left,
	arrow_right,

	print_screen,
	break_,
	pause = break_,

	other,

	buttons_count
};



struct event_t
{

	event_type_t type;

	union
	{
		struct
		{
			mouse_button_t button;
			uint16_t x, y;
		} mouse_button_data;

		struct
		{
			float delta;
			uint16_t x, y; //Cursor position
			bool is_vertical;
		} mouse_scroll_data;

		struct
		{
			keyboard_button_t button;

			//Whether it was pressed with the reported button
			bool alt, control, shift, system;

		} keyboard_button_data;

		struct
		{
			i32 x_new, y_new;
			i32 x_old, y_old;
		} window_move_data;

		struct
		{
			uint16_t width_new, height_new;
			uint16_t width_old, height_old;
		} window_resize_data;

		struct
		{
			i32 x, y;
		} mouse_move_data;

		char32_t character;
	};

	const char* get_type_string() const noexcept
	{
		switch (this->type)
		{
		case event_type_t::create: return "create";
		case event_type_t::close: return "close";
		case event_type_t::move: return "move";
		case event_type_t::resize: return "resize";
		case event_type_t::mouse_press: return "mouse_press";
		case event_type_t::mouse_release: return "mouse_release";
		case event_type_t::mouse_scroll_vertical: return "mouse_scroll_vertical";
		case event_type_t::mouse_scroll_horizontal: return "mouse_scroll_horizontal";
		case event_type_t::mouse_move: return "mouse_move";
		case event_type_t::keyboard_press: return "keyboard_press";
		case event_type_t::keyboard_hold: return "keyboard_hold";
		case event_type_t::keyboard_release: return "keyboard_release";
		case event_type_t::focus_gained: return "focus_gained";
		case event_type_t::focus_lost: return "focus_lost";
		case event_type_t::mouse_entered: return "mouse_entered";
		case event_type_t::mouse_leave: return "mouse_leave";
		case event_type_t::maximized: return "maximized";
		case event_type_t::minimized: return "minimized";
		case event_type_t::text: return "text";
		default: return "<corrupted event>";
		}
	}
};



using window_open_result_t = uint8_t;
struct window_open_result
{
	//All fine
	static constexpr window_open_result_t ok = 0;

	//Not (possibly yet) implemented by the library
	static constexpr window_open_result_t unimplemented = 1;

	//Unexpected OS API invocation error
	static constexpr window_open_result_t os_error = 2;

	//Unexpected OpenGL API invocation error
	static constexpr window_open_result_t opengl_error = 3;

	//OpenGL feature required is not supported
	static constexpr window_open_result_t opengl_unsupported_function = 4;

	//Invalid window size (too big/small)
	static constexpr window_open_result_t size_error = 5;

	//Failed to initialize GLEW
	static constexpr window_open_result_t glew_error = 6;

};



using window_style_t = uint16_t;
struct window_style
{
	static constexpr window_style_t border = 1;
	static constexpr window_style_t close_button = 2;
	static constexpr window_style_t min_button = 4;
	static constexpr window_style_t resize = 8;
	static constexpr window_style_t caption = 16;
	static constexpr window_style_t fullscreen = 32;
	static constexpr window_style_t max_button = 64;
	static constexpr window_style_t hidden = 128;

	static constexpr window_style_t close_min_max = min_button | max_button | close_button;

	static constexpr window_style_t default_style = border | close_button | caption;
};



class window_t;
struct resizemove_data_t
{
	struct
	{
		i32 x_new, y_new;
		i32 x_old, y_old;
	} window_move_data{};

	struct
	{
		uint16_t width_new, height_new;
		uint16_t width_old, height_old;
	} window_resize_data{};

	window_t* window = nullptr;

	bool resize = false;
	bool move = false;
};

using window_resizemove_handle_t = void(*)(const resizemove_data_t*);

_KSN_EXPORT_END
