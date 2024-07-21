
module;

#include <ksn/ksn.hpp>
#include <ksn/internal/window_defs.hpp>

export module libksn.window:submodule_opengl;
import :settings_decl;
import :error_decl;



_KSN_EXPORT_BEGIN

#define module_type graphics
#define module_name opengl
#define module_enum api_enum_from_type(module_type)

#define impl_type impl_class_name(module_type, module_name)
#define settings_type settings_class_name(module_type, module_name)

class impl_type
{
public:
	impl_type(settings_type settings = {}) {} //TODO

	static constexpr module_enum enum_val = module_enum::module_name;

	template<class window_api_t>
	window_operation_result open(u16 width, u16 height, const window_api_t& window_api_impl)
	{
		return graphics_api_error::unimplemented;
	}

	void close()
	{
	}
};

_KSN_EXPORT_END
