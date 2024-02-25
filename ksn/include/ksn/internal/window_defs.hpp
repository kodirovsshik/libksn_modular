
#include <ksn/ksn.hpp>

#ifdef _KSN_COMPILER_MSVC
#define MSVC_MACRO_REDEFINITION_WARNING 4005
/*
	Caused by redefining the X-macro, thus going against the purpore of X-macros
	No #pragma warning(push/pop) used because this file is to be included only
		into files that would want the mentioned warning disabled
*/
#pragma warning(disable : MSVC_MACRO_REDEFINITION_WARNING) 
#endif



#define DTC1(_1)
#define DTC2(_1,_2) _1
#define DTC3(_1,_2,_3) _1,_2
#define DTC4(_1,_2,_3,_4) _1,_2,_3
#define DTC5(_1,_2,_3,_4,_5) _1,_2,_3,_4
#define get_discard_trailing_comma_helper(_1,_2,_3,_4,_5,NAME,...) NAME
#define discard_trailing_comma(...) get_discard_trailing_comma_helper(__VA_ARGS__, DTC5, DTC4, DTC3, DTC2, DTC1)(__VA_ARGS__)



#define nonempty_macro(...) (false __VA_OPT__(|| true))
#define libksn_window_abort_compilation_to_unimplemented_feature(feature) static_assert(false, "libksn.window: " #feature " is not implemented")

#define compose_class_name(prefix, name, postfix) prefix ## _api_ ## name ## postfix

#define settings_class_name(prefix, name) compose_class_name(prefix, name, _settings)
#define impl_class_name(prefix, name) compose_class_name(prefix, name, _impl)



#define Xw(name) X(name, window)

#define _Xw_WINAPI Xw(winapi)
#define _Xw_XLIB Xw(xlib)
#define _Xw_WAYLAND Xw(wayland)

#ifdef _WIN32
#define Xw_WINAPI _Xw_WINAPI
#else
#define Xw_WINAPI
#endif

#ifdef __linux__
#define Xw_XLIB _Xw_XLIB
#else
#define Xw_XLIB
#endif

#ifdef __linux__
#define Xw_WAYLAND _Xw_WAYLAND
#else
#define Xw_WAYLAND
#endif

#define WINDOW_API_XLIST Xw_WAYLAND Xw_XLIB Xw_WINAPI





#define Xg(name) X(name, graphics)

#define _Xg_DIRECTX Xg(directx)
#define _Xg_OPENGL Xg(opengl)
#define _Xg_VULKAN Xg(vulkan)

#define _Xg_NONE Xg(none)

#ifdef _WIN32__stub //TODO
#define Xg_DIRECTX _Xg_DIRECTX
#else
#define Xg_DIRECTX
#endif

#ifdef LIBKSN_WINDOW_ENABLE_OPENGL
#define Xg_OPENGL _Xg_OPENGL
#else
#define Xg_OPENGL
#endif

#ifdef LIBKSN_WINDOW_ENABLE_VULKAN
#define Xg_VULKAN _Xg_VULKAN
#else
#define Xg_VULKAN
#endif

#define Xg_NONE _Xg_NONE

#define GRAPHICS_API_XLIST Xg_NONE Xg_DIRECTX Xg_OPENGL Xg_VULKAN
