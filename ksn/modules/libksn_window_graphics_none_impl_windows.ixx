
module;

#include <ksn/ksn.hpp>
#include <ksn/internal/window_defs.hpp>

export module libksn.window:submodule_graphics_none;
import :settings_decl;
import :error_decl;



_KSN_EXPORT_BEGIN

#define module_type graphics
#define module_name none
#define module_enum api_enum_from_type(module_type)

#define impl_type impl_class_name(module_type, module_name)
#define settings_type settings_class_name(module_type, module_name)

class impl_type
{
	bool direct_draw;

public:
	impl_type(settings_type settings = {}) : direct_draw(settings.request_direct_drawing_support)
	{
	}

	static constexpr module_enum enum_val = module_enum::module_name;

	template<class window_api_t>
	window_operation_result open(u16 width, u16 height, const window_api_t&)
	{
		return this->direct_draw ? graphics_api_error::unimplemented : window_operation_result{};
	}

	void close()
	{
	}
};

_KSN_EXPORT_END
