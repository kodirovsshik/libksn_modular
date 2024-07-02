
module;

#include <ksn/ksn.hpp>
#include <ksn/internal/window_defs.hpp>

export module libksn.window:aux_decl;
import :submodules_decl;
import :submodules_impl;



_KSN_EXPORT_BEGIN

#define common_api_error_xlist X(unimplemented = 1)
#define window_api_error_xlist common_api_error_xlist 
#define graphics_api_error_xlist common_api_error_xlist 

#define X(name) name,
enum class window_api_error
{
	window_api_error_xlist
};

enum class graphics_api_error
{
	graphics_api_error_xlist
};

class window_operation_result
{
	std::optional<window_api_error> w;
	std::optional<graphics_api_error> g;

public:
	window_operation_result() = default;
	window_operation_result(window_api_error err) : w(err) {};
	window_operation_result(graphics_api_error err) : g(err) {};

	explicit operator bool() { return !w.has_value() && !g.has_value(); };

	auto get_window_api_error() { return w; };
	auto get_graphics_api_error() { return g; };
};

_KSN_EXPORT_END

_KSN_BEGIN

struct window_impl_storage
{
	window_api_impl window;
	graphics_api_impl graphics;
	//window_impl impl; //TODO: create common window impl interface
};

_KSN_END