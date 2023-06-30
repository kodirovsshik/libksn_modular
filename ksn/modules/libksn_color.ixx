
export module libksn.color;

import <ksn/ksn.hpp>;



_KSN_EXPORT_BEGIN


struct generic_color_rgb
{
public:
	union
	{
		uint32_t m_value;
		uint8_t byte[4];
		struct
		{
			uint8_t r, g, b, a;
		};
	};
};

struct generic_color_bgr
{
public:
	union
	{
		uint32_t m_value;
		uint8_t byte[4];
		struct
		{
			uint8_t b, g, r, a;
		};
	};
};

struct generic_color_hsv
{
public:
	int16_t m_hue; //[-360; 360]
	uint8_t m_saturation; //[0; 100]
	uint8_t m_value; //[0; 100]
	uint8_t m_alpha; //[0; 255]

	constexpr void hue(int16_t x) noexcept;
	constexpr void saturation(uint8_t x) noexcept;
	constexpr void value(uint8_t x) noexcept;
	constexpr void alpha(uint8_t x) noexcept;

	constexpr int16_t hue() const noexcept;
	constexpr uint8_t saturation() const noexcept;
	constexpr uint8_t value() const noexcept;
	constexpr uint8_t alpha() const noexcept;
};

struct generic_color_hsv_packed
{
public:
	uint32_t data;

	constexpr void hue(int16_t x) noexcept;
	constexpr void saturation(uint8_t x) noexcept;
	constexpr void value(uint8_t x) noexcept;
	constexpr void alpha(uint8_t x) noexcept;

	constexpr int16_t hue() const noexcept;
	constexpr uint8_t saturation() const noexcept;
	constexpr uint8_t value() const noexcept;
	constexpr uint8_t alpha() const noexcept;
};


_KSN_EXPORT_END



_KSN_EXPORT_BEGIN

template<class T>
concept color_some_rgb = requires(T color)
{
	{ color.red() } -> std::same_as<uint8_t>;
	{ color.green() } -> std::same_as<uint8_t>;
	{ color.blue() } -> std::same_as<uint8_t>;
	{ color.alpha() } -> std::same_as<uint8_t>;
};
template<class T>
concept color_bgr = color_some_rgb<T> && requires(T color)
{
	{ color.to_generic_bgr() } -> std::same_as<generic_color_bgr>;
};
template<class T>
concept color_rgb = color_some_rgb<T> && requires(T color)
{
	{ color.to_generic_rgb() } -> std::same_as<generic_color_rgb>;
};

template<class T>
concept color_some_hsv = requires(T color)
{
	{ color.hue() } -> std::same_as<int16_t>;
	{ color.saturation() } -> std::same_as<uint8_t>;
	{ color.value() } -> std::same_as<uint8_t>;
	{ color.alpha() } -> std::same_as<uint8_t>;

	color.hue(int16_t());
	color.saturation(uint8_t());
	color.value(uint8_t());
	color.alpha(uint8_t());

};
template<class T>
concept color_hsv = color_some_hsv<T> && requires(T color)
{
	{ color.to_generic_hsv() } -> std::same_as<generic_color_hsv>;
};
template<class T>
concept color_hsv_packed = color_some_hsv<T> && requires(T color)
{
	{ color.to_generic_hsv_packed() } -> std::same_as<generic_color_hsv_packed>;
};

template<class T>
concept color = color_rgb<T> || color_bgr<T> || color_hsv<T> || color_hsv_packed<T>;


_KSN_EXPORT_END



_KSN_EXPORT_BEGIN


struct color_bgr_t;
struct color_bgra_t;
struct color_rgb_t;
struct color_rgba_t;
struct color_hsv_t;
struct color_hsva_t;
struct color_hsva_packed_t;



struct color_bgr_t
{
	uint8_t b, g, r;


	constexpr color_bgr_t() noexcept;
	constexpr color_bgr_t(uint8_t b, uint8_t g, uint8_t r) noexcept;
	constexpr color_bgr_t(uint32_t hex) noexcept;

	template<color color_t>
	constexpr color_bgr_t(color_t other) noexcept;

	constexpr color_bgr_t& operator=(const color_bgr_t&) noexcept = default;
	constexpr color_bgr_t& operator=(color_bgr_t&&) noexcept = default;

	constexpr color_bgr_t(const color_bgr_t&) noexcept = default;
	constexpr color_bgr_t(color_bgr_t&&) noexcept = default;

	constexpr uint8_t red() const noexcept;
	constexpr uint8_t green() const noexcept;
	constexpr uint8_t blue() const noexcept;
	constexpr uint8_t alpha() const noexcept;

	constexpr generic_color_bgr to_generic_bgr() const noexcept;
};


struct color_bgra_t
{
	union
	{
		struct
		{
			uint8_t b, g, r, a;
		};
		uint32_t value;
	};

	constexpr color_bgra_t() noexcept;
	constexpr color_bgra_t(uint8_t b, uint8_t g, uint8_t r) noexcept;
	constexpr color_bgra_t(uint8_t b, uint8_t g, uint8_t r, uint8_t a) noexcept;
	constexpr color_bgra_t(uint32_t hex) noexcept;
	constexpr color_bgra_t(uint32_t value, int unused) noexcept;

	template<color  color_t>
	constexpr color_bgra_t(color_t other) noexcept;

	constexpr color_bgra_t(const color_bgra_t&) noexcept = default;
	constexpr color_bgra_t(color_bgra_t&&) noexcept = default;

	constexpr color_bgra_t& operator=(const color_bgra_t&) noexcept = default;
	constexpr color_bgra_t& operator=(color_bgra_t&&) noexcept = default;

	constexpr uint8_t red() const noexcept;
	constexpr uint8_t green() const noexcept;
	constexpr uint8_t blue() const noexcept;
	constexpr uint8_t alpha() const noexcept;

	constexpr generic_color_bgr to_generic_bgr() const noexcept;
};


struct color_rgb_t
{
	uint8_t r, g, b;


	constexpr color_rgb_t() noexcept;
	constexpr color_rgb_t(uint8_t r, uint8_t g, uint8_t b) noexcept;
	constexpr color_rgb_t(uint32_t hex) noexcept;

	template<color color_t>
	constexpr color_rgb_t(color_t other) noexcept;

	constexpr color_rgb_t(const color_rgb_t&) noexcept = default;
	constexpr color_rgb_t(color_rgb_t&&) noexcept = default;

	constexpr color_rgb_t& operator=(const color_rgb_t&) noexcept = default;
	constexpr color_rgb_t& operator=(color_rgb_t&&) noexcept = default;

	constexpr uint8_t red() const noexcept;
	constexpr uint8_t green() const noexcept;
	constexpr uint8_t blue() const noexcept;
	constexpr uint8_t alpha() const noexcept;

	constexpr generic_color_rgb to_generic_rgb() const noexcept;
};


struct color_rgba_t
{
	union
	{
		struct
		{
			uint8_t r, g, b, a;
		};
		uint32_t value;
	};

	constexpr color_rgba_t() noexcept;
	constexpr color_rgba_t(uint8_t r, uint8_t g, uint8_t b) noexcept;
	constexpr color_rgba_t(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept;
	constexpr color_rgba_t(uint32_t hex) noexcept;
	constexpr color_rgba_t(uint32_t value, int unused) noexcept;

	template<color color_t>
	constexpr color_rgba_t(color_t other) noexcept;

	constexpr color_rgba_t(const color_rgba_t&) noexcept = default;
	constexpr color_rgba_t(color_rgba_t&&) noexcept = default;

	constexpr color_rgba_t& operator=(const color_rgba_t&) noexcept = default;
	constexpr color_rgba_t& operator=(color_rgba_t&&) noexcept = default;

	constexpr uint8_t red() const noexcept;
	constexpr uint8_t green() const noexcept;
	constexpr uint8_t blue() const noexcept;
	constexpr uint8_t alpha() const noexcept;

	constexpr generic_color_rgb to_generic_rgb() const noexcept;
};


struct color_hsv_t
{
	int16_t m_hue; //[-360; 360]
	uint8_t m_saturation; //[0; 100]
	uint8_t m_value; //[0; 100]


	constexpr int16_t hue() const noexcept;
	constexpr uint8_t saturation() const noexcept;
	constexpr uint8_t value() const noexcept;
	constexpr uint8_t alpha() const noexcept;

	constexpr void hue(uint16_t hue) noexcept;
	constexpr void saturation(uint8_t saturation) noexcept;
	constexpr void value(uint8_t value) noexcept;
	constexpr void alpha(uint8_t alpha) const noexcept;




	constexpr color_hsv_t() noexcept;
	constexpr color_hsv_t(uint16_t hue, uint8_t saturation, uint8_t value) noexcept;

	template<color color_t>
	constexpr color_hsv_t(color_t other) noexcept;

	constexpr color_hsv_t(const color_hsv_t&) noexcept = default;
	constexpr color_hsv_t(color_hsv_t&&) noexcept = default;

	constexpr generic_color_hsv to_generic_hsv() const noexcept;
};


struct color_hsva_t
{
	int16_t m_hue; //[-360; 360]
	uint8_t m_saturation; //[0; 100]
	uint8_t m_value; //[0; 100]
	uint8_t m_alpha; //[0; 255]



	constexpr int16_t hue() const noexcept;
	constexpr uint8_t saturation() const noexcept;
	constexpr uint8_t value() const noexcept;
	constexpr uint8_t alpha() const noexcept;

	constexpr void hue(uint16_t hue) noexcept;
	constexpr void saturation(uint8_t saturation) noexcept;
	constexpr void value(uint8_t value) noexcept;
	constexpr void alpha(uint8_t alpha) noexcept;



	constexpr color_hsva_t() noexcept;
	constexpr color_hsva_t(uint16_t hue, uint8_t saturation, uint8_t value, uint8_t alpha) noexcept;

	template<color color_t>
	constexpr color_hsva_t(color_t other) noexcept;

	constexpr color_hsva_t(const color_hsva_t&) noexcept = default;
	constexpr color_hsva_t(color_hsva_t&&) noexcept = default;

	constexpr generic_color_hsv to_generic_hsv() const noexcept;
};


struct color_hsva_packed_t
{
	uint32_t m_data;


	constexpr int16_t hue() const noexcept;
	constexpr uint8_t saturation() const noexcept;
	constexpr uint8_t value() const noexcept;
	constexpr uint8_t alpha() const noexcept;

	constexpr void hue(int16_t hue) noexcept;
	constexpr void saturation(uint8_t saturation) noexcept;
	constexpr void value(uint8_t value) noexcept;
	constexpr void alpha(uint8_t alpha) noexcept;



	constexpr color_hsva_packed_t() noexcept;
	constexpr color_hsva_packed_t(int16_t hue, uint8_t saturation, uint8_t value) noexcept;
	constexpr color_hsva_packed_t(int16_t hue, uint8_t saturation, uint8_t value, uint8_t alpha) noexcept;

	template<color color_t>
	constexpr color_hsva_packed_t(color_t other) noexcept;

	constexpr color_hsva_packed_t(const color_hsva_packed_t&) noexcept = default;
	constexpr color_hsva_packed_t(color_hsva_packed_t&&) noexcept = default;

	constexpr generic_color_hsv_packed to_generic_hsv_packed() const noexcept;
};


_KSN_EXPORT_END




_KSN_EXPORT_BEGIN

constexpr int16_t hsv_bound_hue(int hue) noexcept
{
	hue %= 360;
	return hue;
}
constexpr int16_t hsv_bound_hue_positive(int hue) noexcept
{
	hue %= 360;
	if (hue < 0)
		hue += 360;
	return hue;
}

_KSN_EXPORT_END





_KSN_BEGIN

constexpr int16_t hsva_packed_extract_hue(uint32_t val)
{
	//return val & 0b1111111111;
	return (val & 0b1111111111) << 6 >> 6; //sign extend
}
constexpr uint8_t hsva_packed_extract_saturation(uint32_t val)
{
	return (val >> 10) & 0b1111111;
}
constexpr uint8_t hsva_packed_extract_value(uint32_t val)
{
	return (val >> 17) & 0b1111111;
}
constexpr uint8_t hsva_packed_extract_alpha(uint32_t val)
{
	return val >> 24;
}

constexpr uint32_t hsva_packed_update_hue(uint32_t data, int16_t val)
{
	constexpr uint32_t shift = 0;
	constexpr uint32_t mask = 0b1111111111 << shift;
	return (data & ~mask) | ((val << shift) & mask);
}
constexpr uint32_t hsva_packed_update_saturation(uint32_t data, uint8_t val)
{
	constexpr uint32_t shift = 10;
	constexpr uint32_t mask = 0b1111111 << shift;
	return (data & ~mask) | ((val << shift) & mask);
}
constexpr uint32_t hsva_packed_update_value(uint32_t data, uint8_t val)
{
	constexpr uint32_t shift = 17;
	constexpr uint32_t mask = 0b1111111 << shift;
	return (data & ~mask) | ((val << shift) & mask);
}
constexpr uint32_t hsva_packed_update_alpha(uint32_t data, uint8_t val)
{
	constexpr uint32_t shift = 24;
	constexpr uint32_t mask = 0xFF << shift;
	return (data & ~mask) | ((val << shift) & mask);
}

_KSN_END





_KSN_BEGIN

constexpr void generic_color_hsv::hue(int16_t x) noexcept
{
	this->m_hue = x;;
}
constexpr void generic_color_hsv::saturation(uint8_t x) noexcept
{
	this->m_saturation = x;
}
constexpr void generic_color_hsv::value(uint8_t x) noexcept
{
	this->m_value = x;
}
constexpr void generic_color_hsv::alpha(uint8_t x) noexcept
{
	this->m_alpha = x;
}

constexpr int16_t generic_color_hsv::hue() const noexcept
{
	return this->m_hue;
}
constexpr uint8_t generic_color_hsv::saturation() const noexcept
{
	return this->m_saturation;
}
constexpr uint8_t generic_color_hsv::value() const noexcept
{
	return this->m_value;
}
constexpr uint8_t generic_color_hsv::alpha() const noexcept
{
	return this->m_alpha;
}



constexpr void generic_color_hsv_packed::hue(int16_t hue) noexcept
{
	this->data = hsva_packed_update_hue(this->data, hue);
}
constexpr void generic_color_hsv_packed::saturation(uint8_t saturation) noexcept
{
	this->data = hsva_packed_update_saturation(this->data, saturation);
}
constexpr void generic_color_hsv_packed::value(uint8_t value) noexcept
{
	this->data = hsva_packed_update_value(this->data, value);
}
constexpr void generic_color_hsv_packed::alpha(uint8_t alpha) noexcept
{
	this->data = hsva_packed_update_alpha(this->data, alpha);
}

constexpr int16_t generic_color_hsv_packed::hue() const noexcept
{
	return hsva_packed_extract_hue(this->data);
}
constexpr uint8_t generic_color_hsv_packed::saturation() const noexcept
{
	return hsva_packed_extract_saturation(this->data);
}
constexpr uint8_t generic_color_hsv_packed::value() const noexcept
{
	return hsva_packed_extract_value(this->data);
}
constexpr uint8_t generic_color_hsv_packed::alpha() const noexcept
{
	return hsva_packed_extract_alpha(this->data);
}

_KSN_END





_KSN_BEGIN

_KSN_DETAIL_BEGIN

template<int divisor>
constexpr int rounded_quotient_of_division_by(int dividend)
{
	int q = dividend / divisor;
	int r = dividend % divisor;
	if (r < 0)
		r += divisor;
	if (r > (divisor - 1) / 2)
		q++;
	return q;
}
constexpr int rounded_quotient_of_division_by(int divisor, int dividend)
{
	int q = dividend / divisor;
	int r = dividend % divisor;
	if (r < 0)
	{
		r += divisor;
		q--;
	}
	if (r > (divisor - 1) / 2)
		q++;
	return q;
}

template<int divisor>
constexpr int remainder_of_division_by(int dividend)
{
	dividend %= divisor;
	if (dividend < 0)
		dividend += divisor;
	return dividend;
}

constexpr int _linear_360_oscilator_abs(int x)
{
	return x < 0 ? -x : x;
}

constexpr int linear_360_oscilator(int x)
{
	x = remainder_of_division_by<360>(x);
	return _linear_360_oscilator_abs(180 - x);
}

constexpr int convert_hsv_rgb_oscilator(int x)
{
	return linear_360_oscilator(x) + linear_360_oscilator(x + 120) - 120;
}

constexpr int convert_hsv_rgb_helper(int x, int S, int V)
{
	x = V * (12000 - S * (120 - x));
	return rounded_quotient_of_division_by<80000>(x);
}

_KSN_DETAIL_END

template<class some_rgba>
constexpr some_rgba convert_hsv_rgb(int H, int S, int V, uint8_t a)
{
	some_rgba rgba;
	rgba.a = a;

	V *= 17;
	rgba.r = (uint8_t)detail::convert_hsv_rgb_helper(detail::convert_hsv_rgb_oscilator(H - 60), S, V);
	rgba.g = (uint8_t)detail::convert_hsv_rgb_helper(detail::convert_hsv_rgb_oscilator(H - 180), S, V);
	rgba.b = (uint8_t)detail::convert_hsv_rgb_helper(detail::convert_hsv_rgb_oscilator(H + 60), S, V);
	return rgba;
}


template<color_some_hsv some_hsv>
constexpr some_hsv convert_rgb_hsv(int r, int g, int b, uint8_t a)
{
	some_hsv result{};
	const auto [min, max] = std::minmax({ r, g, b });
	auto delta = max - min;
	int h;
	if (delta == 0)
		h = 0;
	else if (max == r)
		h = detail::rounded_quotient_of_division_by(delta, detail::remainder_of_division_by<360>(60 * (g - b)));
	else if (max == g)
		h = detail::rounded_quotient_of_division_by(delta, 60 * (b - r)) + 120;
	else /*if (max == b)*/
		h = detail::rounded_quotient_of_division_by(delta, 60 * (r - g)) + 240;
	result.hue(h);

	if (max == 0)
		result.saturation(0);
	else
		result.saturation(detail::rounded_quotient_of_division_by(max, delta * 100));

	result.value(detail::rounded_quotient_of_division_by<255>(max * 100));
	result.alpha(a);

	return result;
}

_KSN_END





_KSN_BEGIN

_KSN_DETAIL_BEGIN

template<class color_t>
constexpr generic_color_bgr generic_color_convert_bgr2(color_t) noexcept
{
	return {};
}
template<color_some_hsv color_t>
constexpr generic_color_bgr generic_color_convert_bgr2(color_t other) noexcept
{
	return convert_hsv_rgb<generic_color_bgr>(other.hue(), other.saturation(), other.value(), other.alpha());
}
template<class color_t>
constexpr generic_color_bgr generic_color_convert_bgr1(color_t other) noexcept
{
	return generic_color_convert_bgr2(other);
}
template<color_rgb color_t>
constexpr generic_color_bgr generic_color_convert_bgr1(color_t other) noexcept
{
	auto rgb = other.to_generic_rgb();

	generic_color_bgr bgr;
	bgr.r = rgb.r;
	bgr.g = rgb.g;
	bgr.b = rgb.b;
	bgr.a = rgb.a;

	return bgr;
}



template<class color_t>
constexpr generic_color_rgb generic_color_convert_rgb2(color_t) noexcept
{
	return {};
}
template<color_some_hsv color_t>
constexpr generic_color_rgb generic_color_convert_rgb2(color_t other) noexcept
{
	return convert_hsv_rgb<generic_color_rgb>(other.hue(), other.saturation(), other.value(), other.alpha());
}
template<class color_t>
constexpr generic_color_rgb generic_color_convert_rgb1(color_t other)
{
	return generic_color_convert_rgb2(other);
}
template<color_bgr color_t>
constexpr generic_color_rgb generic_color_convert_rgb1(color_t other)
{
	auto bgr = other.to_generic_bgr();

	generic_color_rgb rgb;
	rgb.r = bgr.r;
	rgb.g = bgr.g;
	rgb.b = bgr.b;
	rgb.a = bgr.a;

	return rgb;
}



template<class color_t>
constexpr generic_color_hsv generic_color_convert_hsv2(color_t other) noexcept
{
	return {};
}
template<color_some_rgb color_t>
constexpr generic_color_hsv generic_color_convert_hsv2(color_t other) noexcept
{
	return convert_rgb_hsv<generic_color_hsv>(other.red(), other.green(), other.blue(), other.alpha());
}

template<class color_t>
constexpr generic_color_hsv generic_color_convert_hsv1(color_t other) noexcept
{
	return generic_color_convert_hsv2(other);
}
template<color_hsv_packed color_t>
constexpr generic_color_hsv generic_color_convert_hsv1(color_t other) noexcept
{
	auto packed = other.to_generic_hsv_packed();

	generic_color_hsv result;
	result.m_hue = hsva_packed_extract_hue(packed.data);
	result.m_saturation = hsva_packed_extract_saturation(packed.data);
	result.m_value = hsva_packed_extract_value(packed.data);
	result.m_alpha = hsva_packed_extract_alpha(packed.data);

	return result;
}



template<class color_t>
constexpr generic_color_hsv_packed generic_color_convert_hsv_packed2(color_t other) noexcept
{
	return {};
}
template<color_some_rgb color_t>
constexpr generic_color_hsv_packed generic_color_convert_hsv_packed2(color_t other) noexcept
{
	return convert_rgb_hsv<generic_color_hsv_packed>(other.red(), other.green(), other.blue(), other.alpha());
}

template<class color_t>
constexpr generic_color_hsv_packed generic_color_convert_hsv_packed1(color_t other) noexcept
{
	return detail::generic_color_convert_hsv_packed2(other);
}
template<color_hsv color_t>
constexpr generic_color_hsv_packed generic_color_convert_hsv_packed1(color_t other) noexcept
{
	ksn::generic_color_hsv hsv = other.to_generic_hsv();

	generic_color_hsv_packed result{};
	result.data = hsva_packed_update_hue(result.data, hsv.hue());
	result.data = hsva_packed_update_saturation(result.data, hsv.saturation());
	result.data = hsva_packed_update_value(result.data, hsv.value());
	result.data = hsva_packed_update_alpha(result.data, hsv.alpha());
	return result;
}

_KSN_DETAIL_END

template<class color_t>
constexpr generic_color_bgr generic_color_convert_bgr(color_t other) noexcept
{
	return detail::generic_color_convert_bgr1(other);
}
template<color_bgr color_t>
constexpr generic_color_bgr generic_color_convert_bgr(color_t other) noexcept
{
	return other.to_generic_bgr();
}


template<class color_t>
constexpr generic_color_rgb generic_color_convert_rgb(color_t other) noexcept
{
	return detail::generic_color_convert_rgb1(other);
}
template<color_rgb color_t>
constexpr generic_color_rgb generic_color_convert_rgb(color_t other) noexcept
{
	return other.to_generic_rgb();
}


template<class color_t>
constexpr generic_color_hsv generic_color_convert_hsv(color_t other) noexcept
{
	return detail::generic_color_convert_hsv1(other);
}
template<color_hsv color_t>
constexpr generic_color_hsv generic_color_convert_hsv(color_t other) noexcept
{
	return other.to_generic_hsv();
}


template<class color_t>
constexpr generic_color_hsv_packed generic_color_convert_hsv_packed(color_t other) noexcept
{
	return detail::generic_color_convert_hsv_packed1(other);
}
template<color_hsv_packed color_t>
constexpr generic_color_hsv_packed generic_color_convert_hsv_packed(color_t other) noexcept
{
	return other.to_generic_hsv_packed();
}

_KSN_END





_KSN_BEGIN

constexpr color_bgr_t::color_bgr_t() noexcept
{
	this->r = this->b = this->g = 0;
}
constexpr color_bgr_t::color_bgr_t(uint8_t b, uint8_t g, uint8_t r) noexcept
{
	this->b = b;
	this->g = g;
	this->r = r;
}
constexpr color_bgr_t::color_bgr_t(uint32_t hex) noexcept
{
	this->b = hex & 0xFF;
	this->g = (hex >> 8) & 0xFF;
	this->r = (hex >> 16) & 0xFF;
}

template<color color_t>
constexpr color_bgr_t::color_bgr_t(color_t other) noexcept
{
	auto bgr = generic_color_convert_bgr(other);
	this->b = bgr.b;
	this->g = bgr.g;
	this->r = bgr.r;
}

constexpr uint8_t color_bgr_t::red() const noexcept
{
	return this->r;
}
constexpr uint8_t color_bgr_t::green() const noexcept
{
	return this->g;
}
constexpr uint8_t color_bgr_t::blue() const noexcept
{
	return this->b;
}
constexpr uint8_t color_bgr_t::alpha() const noexcept
{
	return 255;
}
constexpr generic_color_bgr color_bgr_t::to_generic_bgr() const noexcept
{
	generic_color_bgr bgr;
	bgr.b = this->b;
	bgr.g = this->g;
	bgr.r = this->r;
	bgr.a = 255;
	return bgr;
}

_KSN_END





_KSN_BEGIN

constexpr color_bgra_t::color_bgra_t() noexcept
{
	this->value = 0;
}
constexpr color_bgra_t::color_bgra_t(uint8_t b, uint8_t g, uint8_t r) noexcept
{
	this->b = b;
	this->g = g;
	this->r = r;
	this->a = 255;
}
constexpr color_bgra_t::color_bgra_t(uint8_t b, uint8_t g, uint8_t r, uint8_t a) noexcept
{
	this->b = b;
	this->g = g;
	this->r = r;
	this->a = a;
}
constexpr color_bgra_t::color_bgra_t(uint32_t hex) noexcept
{
	this->value = hex | 0xFF000000;
}
constexpr color_bgra_t::color_bgra_t(uint32_t value, int unused) noexcept
{
	this->value = value;
	(void)unused;
}

template<color  color_t>
constexpr color_bgra_t::color_bgra_t(color_t other) noexcept
{
	auto bgr = generic_color_convert_bgr(other);
	this->value = bgr.m_value;
}

constexpr uint8_t color_bgra_t::red() const noexcept
{
	return this->r;
}
constexpr uint8_t color_bgra_t::green() const noexcept
{
	return this->g;
}
constexpr uint8_t color_bgra_t::blue() const noexcept
{
	return this->b;
}
constexpr uint8_t color_bgra_t::alpha() const noexcept
{
	return this->a;
}

constexpr generic_color_bgr color_bgra_t::to_generic_bgr() const noexcept
{
	generic_color_bgr bgr;
	bgr.m_value = this->value;
	return bgr;
}

_KSN_END





_KSN_BEGIN

constexpr color_rgb_t::color_rgb_t() noexcept
{
	this->r = this->g = this->b = 0;
}

constexpr color_rgb_t::color_rgb_t(uint8_t r, uint8_t g, uint8_t b) noexcept
{
	this->r = r;
	this->g = g;
	this->b = b;
}
constexpr color_rgb_t::color_rgb_t(uint32_t hex) noexcept
{
	this->b = hex & 0xFF;
	this->g = (hex >> 8) & 0xFF;
	this->r = (hex >> 16) & 0xFF;
}

template<color color_t>
constexpr color_rgb_t::color_rgb_t(color_t other) noexcept
{
	auto bgr = generic_color_convert_rgb(other);
	this->r = bgr.r;
	this->g = bgr.g;
	this->b = bgr.b;
}

constexpr uint8_t color_rgb_t::red() const noexcept
{
	return this->r;
}
constexpr uint8_t color_rgb_t::green() const noexcept
{
	return this->g;
}
constexpr uint8_t color_rgb_t::blue() const noexcept
{
	return this->b;
}
constexpr uint8_t color_rgb_t::alpha() const noexcept
{
	return 255;
}

constexpr generic_color_rgb color_rgb_t::to_generic_rgb() const noexcept
{
	generic_color_rgb x;
	x.r = this->r;
	x.g = this->g;
	x.b = this->b;
	x.a = 255;
	return x;
}

_KSN_END





_KSN_BEGIN

constexpr color_rgba_t::color_rgba_t() noexcept
{
	this->value = 0;
}
constexpr color_rgba_t::color_rgba_t(uint8_t r, uint8_t g, uint8_t b) noexcept
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 255;
}
constexpr color_rgba_t::color_rgba_t(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}
constexpr color_rgba_t::color_rgba_t(uint32_t hex) noexcept
{
	this->value = hex | 0xFF000000;
}
constexpr color_rgba_t::color_rgba_t(uint32_t value, int unused) noexcept
{
	this->value = value;
	(void)unused;
}

template<color color_t>
constexpr color_rgba_t::color_rgba_t(color_t other) noexcept
{
	auto rgb = generic_color_convert_rgb(other);
	this->value = rgb.m_value;
}

constexpr uint8_t color_rgba_t::red() const noexcept
{
	return this->r;
}
constexpr uint8_t color_rgba_t::green() const noexcept
{
	return this->g;
}
constexpr uint8_t color_rgba_t::blue() const noexcept
{
	return this->b;
}
constexpr uint8_t color_rgba_t::alpha() const noexcept
{
	return this->a;
}

constexpr generic_color_rgb color_rgba_t::to_generic_rgb() const noexcept
{
	generic_color_rgb x;
	x.m_value = this->value;
	return x;
}

_KSN_END





_KSN_BEGIN

constexpr int16_t color_hsv_t::hue() const noexcept
{
	return this->m_hue;
}
constexpr uint8_t color_hsv_t::saturation() const noexcept
{
	return this->m_saturation;
}
constexpr uint8_t color_hsv_t::value() const noexcept
{
	return this->m_value;
}
constexpr uint8_t color_hsv_t::alpha() const noexcept
{
	return 255;
}

constexpr void color_hsv_t::hue(uint16_t hue) noexcept
{
	this->m_hue = hue;
}
constexpr void color_hsv_t::saturation(uint8_t saturation) noexcept
{
	this->m_saturation = saturation;
}
constexpr void color_hsv_t::value(uint8_t value) noexcept
{
	this->m_value = value;
}
constexpr void color_hsv_t::alpha(uint8_t) const noexcept
{
	/* no-op */
}



constexpr color_hsv_t::color_hsv_t() noexcept
{
	this->m_hue = this->m_saturation = this->m_value = 0;
}
constexpr color_hsv_t::color_hsv_t(uint16_t hue, uint8_t saturation, uint8_t value) noexcept
{
	this->m_hue = hue;
	this->m_saturation = saturation;
	this->m_value = value;
}

template<color color_t>
constexpr color_hsv_t::color_hsv_t(color_t other) noexcept
{
	auto hsv = generic_color_convert_hsv(other);

	this->hue(hsv.hue());
	this->saturation(hsv.saturation());
	this->value(hsv.value());
	this->alpha(other.alpha());
}

constexpr generic_color_hsv color_hsv_t::to_generic_hsv() const noexcept
{
	generic_color_hsv val;
	val.m_hue = this->m_hue;
	val.m_saturation = this->m_saturation;
	val.m_value = this->m_value;
	val.m_alpha = 255;
	return val;
}

_KSN_END





_KSN_BEGIN

constexpr int16_t color_hsva_t::hue() const noexcept
{
	return this->m_hue;
}
constexpr uint8_t color_hsva_t::saturation() const noexcept
{
	return this->m_saturation;
}
constexpr uint8_t color_hsva_t::value() const noexcept
{
	return this->m_value;
}
constexpr uint8_t color_hsva_t::alpha() const noexcept
{
	return this->m_alpha;
}

constexpr void color_hsva_t::hue(uint16_t hue) noexcept
{
	this->m_hue = hue;
}
constexpr void color_hsva_t::saturation(uint8_t saturation) noexcept
{
	this->m_saturation = saturation;
}
constexpr void color_hsva_t::value(uint8_t value) noexcept
{
	this->m_value = value;
}
constexpr void color_hsva_t::alpha(uint8_t alpha) noexcept
{
	this->m_alpha = alpha;
}



constexpr color_hsva_t::color_hsva_t() noexcept
{
	this->m_hue = 0;
	this->m_saturation = 0;
	this->m_value = 0;
	this->m_alpha = 0;
}
constexpr color_hsva_t::color_hsva_t(uint16_t hue, uint8_t saturation, uint8_t value, uint8_t alpha) noexcept
{
	this->m_hue = hue;
	this->m_saturation = saturation;
	this->m_value = value;
	this->m_alpha = alpha;
}

template<color color_t>
constexpr color_hsva_t::color_hsva_t(color_t other) noexcept
{
	auto hsv = generic_color_convert_hsv(other);
	
	this->hue(hsv.hue());
	this->saturation(hsv.saturation());
	this->value(hsv.value());
	this->alpha(hsv.alpha());
}

constexpr generic_color_hsv color_hsva_t::to_generic_hsv() const noexcept
{
	generic_color_hsv result{};
	result.m_hue = this->m_hue;
	result.m_saturation = this->m_saturation;
	result.m_value = this->m_value;
	result.m_alpha = this->m_alpha;
	return result;
}

_KSN_END





_KSN_BEGIN


constexpr int16_t color_hsva_packed_t::hue() const noexcept
{
	return hsva_packed_extract_hue(this->m_data);
}
constexpr uint8_t color_hsva_packed_t::saturation() const noexcept
{
	return hsva_packed_extract_saturation(this->m_data);
}
constexpr uint8_t color_hsva_packed_t::value() const noexcept
{
	return hsva_packed_extract_value(this->m_data);
}
constexpr uint8_t color_hsva_packed_t::alpha() const noexcept
{
	return hsva_packed_extract_alpha(this->m_data);
}

constexpr void color_hsva_packed_t::hue(int16_t hue) noexcept
{
	this->m_data = hsva_packed_update_hue(this->m_data, hue);
}
constexpr void color_hsva_packed_t::saturation(uint8_t saturation) noexcept
{
	this->m_data = hsva_packed_update_saturation(this->m_data, saturation);
}
constexpr void color_hsva_packed_t::value(uint8_t value) noexcept
{
	this->m_data = hsva_packed_update_value(this->m_data, value);
}
constexpr void color_hsva_packed_t::alpha(uint8_t alpha) noexcept
{
	this->m_data = hsva_packed_update_alpha(this->m_data, alpha);
}



constexpr color_hsva_packed_t::color_hsva_packed_t() noexcept
{
	this->m_data = 0;
}
constexpr color_hsva_packed_t::color_hsva_packed_t(int16_t hue, uint8_t saturation, uint8_t value) noexcept
{
	this->hue(hue);
	this->saturation(saturation);
	this->value(value);
	this->alpha(255);
}
constexpr color_hsva_packed_t::color_hsva_packed_t(int16_t hue, uint8_t saturation, uint8_t value, uint8_t alpha) noexcept
{
	this->hue(hue);
	this->saturation(saturation);
	this->value(value);
	this->alpha(alpha);
}

template<color color_t>
constexpr color_hsva_packed_t::color_hsva_packed_t(color_t other) noexcept
{
	auto hsv = generic_color_convert_hsv_packed(other);
	this->m_data = hsv.data;
}

constexpr generic_color_hsv_packed color_hsva_packed_t::to_generic_hsv_packed() const noexcept
{
	return generic_color_hsv_packed{ this->m_data };
}

_KSN_END
