
module;

#include <ksn/ksn.hpp>

export module libksn.window:main_module;
import :submodules_decl;
import :submodules_impl;
import :aux_decl;



_KSN_EXPORT_BEGIN

class window_t
{
public:
	window_t(graphics_api_settings = ksn::graphics_api::default_, window_api_settings = ksn::window_api::default_);

	template<class CharT = char>
	window_operation_result open(uint16_t width, uint16_t height, const CharT* title = "");

private:
	struct window_impl_storage impl;
};

_KSN_EXPORT_END
