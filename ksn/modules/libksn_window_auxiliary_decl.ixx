
module;

#include <ksn/ksn.hpp>
#include <ksn/internal/window_defs.hpp>

export module libksn.window:aux_decl;
import :submodules_decl;
import :submodules_impl;



_KSN_BEGIN

struct window_impl_storage
{
	window_api_impl window;
	graphics_api_impl graphics;
	//window_impl impl; //TODO: create common window impl interface
};

_KSN_END
