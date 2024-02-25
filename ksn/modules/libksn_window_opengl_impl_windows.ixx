
module;

#include <ksn/ksn.hpp>
#include <ksn/internal/window_defs.hpp>

export module libksn.window:submodule_opengl;
import :settings_decl;



_KSN_EXPORT_BEGIN

#define module_type graphics
#define module_name opengl

#define impl_type impl_class_name(module_type, module_name)
#define settings_type settings_class_name(module_type, module_name)

class impl_type
{
public:
	impl_type(settings_type settings = {}) {} //TODO
};

_KSN_EXPORT_END
