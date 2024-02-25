
module;

#include <ksn/ksn.hpp>

export module libksn.window:main_module;
import :submodules_decl;
import :submodules_impl;

_KSN_EXPORT_BEGIN

class window_t
{
public:
	window_t(graphics_api_settings = ksn::graphics_api::default_, window_api_settings = ksn::window_api::default_);


private:
	window_api_impl impl_w;
	graphics_api_impl impl_g;
	//class window_impl impl; //TODO: create common window impl interface
};

_KSN_EXPORT_END


