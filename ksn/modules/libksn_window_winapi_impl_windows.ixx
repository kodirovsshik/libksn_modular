
module;

#include <ksn/ksn.hpp>
#include <ksn/internal/window_defs.hpp>

#include <Windows.h>

export module libksn.window:submodule_winapi;
import :settings_decl;
import :error_decl;



#define module_type window
#define module_name winapi
#define module_enum api_enum_from_type(module_type)

#define impl_type impl_class_name(module_type, module_name)
#define settings_type settings_class_name(module_type, module_name)



_KSN_BEGIN

LRESULT winapi_window_procedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProcW(hWnd, Msg, wParam, lParam);
}

constexpr const wchar_t* winapi_window_class_name = L"LIBKSN_WINDOW";
const auto winapi_instance = GetModuleHandleW(NULL);

const int init = [] {
	WNDCLASSEXW window_class_info{};
	window_class_info.cbSize = sizeof(window_class_info);
	window_class_info.style = CS_GLOBALCLASS | CS_OWNDC;
	window_class_info.lpfnWndProc = winapi_window_procedure;
	window_class_info.hInstance = winapi_instance;
	window_class_info.hCursor = LoadCursorW(winapi_instance, IDC_ARROW);
	window_class_info.lpszClassName = winapi_window_class_name;
	auto register_result = RegisterClassExW(&window_class_info);

	return 0;
}();

_KSN_END



_KSN_EXPORT_BEGIN

class impl_type
{
	HWND window = nullptr;

public:
	impl_type(settings_type settings = {}) {}

	static constexpr module_enum enum_val = module_enum::module_name;

	template<class CharT, class graphics_api_t>
	window_operation_result open(u16 width, u16 height, const CharT* name, const graphics_api_t& graphics_api_impl)
	{
		this->close();

		const wchar_t* wname = L"test"; //TODO
		const int style = WS_OVERLAPPEDWINDOW | WS_VISIBLE; //TODO

		//TODO: AdjustClientRect or something

		this->window = CreateWindowW(
			winapi_window_class_name,
			wname,
			style,
			CW_USEDEFAULT, CW_USEDEFAULT,
			width, height,
			NULL, NULL, winapi_instance,
			(void*)this
		);

		return window_api_error::unimplemented;
	}

	void close()
	{
		DestroyWindow(this->window);
		this->window = NULL;
	}
};

_KSN_EXPORT_END
