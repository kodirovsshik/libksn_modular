
module;

#include <ksn/ksn.hpp>
#include <ksn/internal/window_defs.hpp>

export module libksn.window:submodules_decl;
import :settings_decl;




_KSN_BEGIN

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
