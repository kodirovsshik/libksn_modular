
module;

#include <ksn/ksn.hpp>
#include <ksn/internal/window_defs.hpp>

export module libksn.window:modules_declarations;



#pragma warning(disable : 4005) //Macro redefinition


_KSN_EXPORT_BEGIN

#define X(name, type) settings_class_name(type, name)

class X_WINAPI {};
class X_XLIB {};
class X_WAYLAND {};

class Xg_NONE
{
	bool request_direct_drawing_support = false;
};
class X_OPENGL
{
	uint8_t ogl_major = 1;
	uint8_t ogl_minor = 1;
	bool core_profile = false;
	bool debug_profile = false;
};
class X_DIRECTX {};
class X_VULKAN {};

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

_KSN_EXPORT_END



#define DTC1(_1)
#define DTC2(_1,_2) _1
#define DTC3(_1,_2,_3) _1,_2
#define DTC4(_1,_2,_3,_4) _1,_2,_3
#define DTC5(_1,_2,_3,_4,_5) _1,_2,_3,_4
#define GET_DTC(_1,_2,_3,_4,_5,NAME,...) NAME
#define discard_trailing_comma(...) GET_DTC(__VA_ARGS__, DTC5, DTC4, DTC3, DTC2, DTC1)(__VA_ARGS__)


_KSN_EXPORT_BEGIN

#define X(name, type) class settings_class_name(type, name),
using window_api_settings = std::variant<window_api, discard_trailing_comma(WINDOW_API_XLIST)>;
using graphics_api_settings = std::variant<graphics_api, discard_trailing_comma(GRAPHICS_API_XLIST)>;

_KSN_EXPORT_END



export _KSN_BEGIN

#define X(name, type) class impl_class_name(type, name);
WINDOW_API_XLIST;
GRAPHICS_API_XLIST;



#define X(name, type) class impl_class_name(type, name),

using window_api_impl = std::variant<discard_trailing_comma(WINDOW_API_XLIST)>;
using graphics_api_impl = std::variant<discard_trailing_comma(GRAPHICS_API_XLIST)>;



template<class settings_t>
struct map_settings_to_impl {};

template<class settings_t>
using map_settings_to_impl_t = map_settings_to_impl<settings_t>::type;

#define X(name, type_) template<> struct map_settings_to_impl<settings_class_name(type_, name)> { using type = impl_class_name(type_, name); };

WINDOW_API_XLIST;
GRAPHICS_API_XLIST;

_KSN_END
