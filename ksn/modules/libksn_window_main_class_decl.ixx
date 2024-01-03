
module;

#include <ksn/ksn.hpp>

export module libksn.window:main_declarations;
export import :modules_declarations;
import :modules_implementation;

_KSN_EXPORT_BEGIN

class window_t
{
public:


private:
	window_api_impl impl_w;
	graphics_api_impl impl_g;
	class window_impl impl;
};

_KSN_EXPORT_END
