
module;

#include <ksn/ksn.hpp>
#include <ksn/internal/window_defs.hpp>

export module libksn.window:settings_decl;


_KSN_EXPORT_BEGIN

#define X(name, type) settings_class_name(type, name)



#if nonempty_macro(Xw_WINAPI)
class _Xw_WINAPI {};
#endif

#if nonempty_macro(Xw_XLIB)
libksn_window_abort_compilation_to_unimplemented_feature("Xlib support");
class _Xw_XLIB {};
#endif

#if nonempty_macro(Xw_WAYLAND)
libksn_window_abort_compilation_to_unimplemented_feature("Wayland support");
class _Xw_WAYLAND {};
#endif



#if nonempty_macro(Xg_NONE)
class _Xg_NONE
{
	bool request_direct_drawing_support = false;
};
#endif

#if nonempty_macro(Xg_OPENGL)
class _Xg_OPENGL
{
	uint8_t ogl_version_major = 1;
	uint8_t ogl_version_minor = 1;
	bool core_profile = false;
	bool debug_profile = false;
};
#endif

#if nonempty_macro(Xg_DIRECTX)
libksn_window_abort_compilation_to_unimplemented_feature("DirectX support");
class _Xg_DIRECTX {};
#endif

#if nonempty_macro(Xg_VULKAN)
libksn_window_abort_compilation_to_unimplemented_feature("Vulkan support");
class _Xg_VULKAN {};
#endif

_KSN_EXPORT_END



_KSN_EXPORT_BEGIN

#define X(name, _) name,

enum class window_api {
	WINDOW_API_XLIST
	default_ = 0
};

enum class graphics_api {
	GRAPHICS_API_XLIST
	default_ = none
};



#define X(name, type) class settings_class_name(type, name),
using window_api_settings = std::variant<window_api, discard_trailing_comma(WINDOW_API_XLIST)>;
using graphics_api_settings = std::variant<graphics_api, discard_trailing_comma(GRAPHICS_API_XLIST)>;

_KSN_EXPORT_END
