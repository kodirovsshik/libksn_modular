
module;

#include <ksn/ksn.hpp>

export module libksn.window:error_decl;



_KSN_EXPORT_BEGIN

#define common_api_error_xlist unimplemented = 1,
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
	std::variant<std::true_type, window_api_error, graphics_api_error> val = std::true_type{};

	template<class T>
	bool test(T x) const { return std::holds_alternative<T>(val) && std::get<T>(val) == x; }

public:
	window_operation_result() = default;
	window_operation_result(window_api_error err) : val(err) {};
	window_operation_result(graphics_api_error err) : val(err) {};

	explicit operator bool() { return std::holds_alternative<std::true_type>(val); };

	bool operator==(window_api_error w) const { return test(w); };
	bool operator==(graphics_api_error g) const { return test(g); };
};

_KSN_EXPORT_END
