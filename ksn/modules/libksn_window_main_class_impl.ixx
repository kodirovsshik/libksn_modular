
module;

#include <ksn/ksn.hpp>
#include <ksn/internal/window_defs.hpp>

export module libksn.window:main_module_impl;
import :main_module;
import :submodules_decl;



_KSN_BEGIN

template<class... Bases>
struct overload : public Bases...
{
};

template<class settings_t, class impl_t>
concept some_api_settings = requires(settings_t settings)
{
	{ map_settings_to_impl_t<settings_t>(settings) } -> std::convertible_to<impl_t>;
};

template<class impl_t>
struct mapping_settings_visiter_t
{
	template<some_api_settings<impl_t> T>
	impl_t operator()(T settings)
	{
		return map_settings_to_impl_t<T>(settings);
	}
};

auto graphics_impl_factory(graphics_api_settings settings)
{
	auto graphics_api_enum_visiter = []
	(graphics_api choice) -> graphics_api_impl
		{
			switch (choice)
			{
			case graphics_api::none:
				return graphics_api_none_impl();

#if nonempty_macro(Xg_OPENGL)
			case graphics_api::opengl:
				return graphics_api_opengl_impl();
#endif
			}
			std::unreachable();
		};

	auto visiter = overload{ graphics_api_enum_visiter, mapping_settings_visiter_t<graphics_api_impl>{} };

	return std::visit(visiter, settings);
}
auto window_impl_factory(window_api_settings settings)
{
	auto window_api_enum_visiter = []
	(window_api choice) -> window_api_impl
		{
			switch (choice)
			{
#if nonempty_macro(Xw_WINAPI)
			case window_api::winapi:
				return window_api_winapi_impl();
#endif
			}
			std::unreachable();
		};

	auto visiter = overload{ window_api_enum_visiter, mapping_settings_visiter_t<window_api_impl>{} };

	return std::visit(visiter, settings);
}

_KSN_END



_KSN_EXPORT_BEGIN

window_t::window_t(graphics_api_settings graphics_settings, window_api_settings window_settings)
	: impl{ window_impl_factory(window_settings), graphics_impl_factory(graphics_settings) }
{
}
template<class CharT>
window_operation_result window_t::open(uint16_t width, uint16_t height, const CharT* title)
{
	return window_api_error::unimplemented;
}

#define create_window_open_instantiation(T) create_instantiation(T, window_operation_result window_t::open, uint16_t width, uint16_t height, const char* title)
//WHY DO I HAVE TO WRITE THAT TO MAKE MY MODULES LINK PROPERLY
create_window_open_instantiation(char);

_KSN_EXPORT_END
