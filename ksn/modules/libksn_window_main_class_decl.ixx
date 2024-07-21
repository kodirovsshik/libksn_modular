
module;

#include <ksn/ksn.hpp>

export module libksn.window:main_module;
import :submodules_decl;
import :submodules_impl;
import :aux_decl;



_KSN_EXPORT_BEGIN

class window_t : public uncopyable
{
public:
	window_t(graphics_api_settings = ksn::graphics_api::default_, window_api_settings = ksn::window_api::default_) noexcept;

	template<class CharT = char>
	window_operation_result open(u16 width, u16 height, const CharT* title = "") noexcept;

	void close() noexcept;

	window_api get_window_api_type() const noexcept;
	graphics_api get_graphics_api_type() const noexcept;

private:
	struct window_impl_storage impl;
};

_KSN_EXPORT_END
