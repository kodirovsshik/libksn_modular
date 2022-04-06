﻿
export module libksn.color;

import <ksn/ksn.hpp>;
import <concepts>;



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
	uint8_t alpha; //[0; 255]
};

struct generic_color_hsv_packed
{
public:
	uint32_t data;
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
struct color_hsv_packed_t;
struct color_hsva_packed_t;



struct color_bgr_t
{
	uint8_t b, g, r;


	constexpr color_bgr_t() noexcept;
	constexpr color_bgr_t(uint8_t r, uint8_t g, uint8_t b) noexcept;
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
	constexpr color_bgra_t(uint8_t r, uint8_t g, uint8_t b) noexcept;
	constexpr color_bgra_t(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept;
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

	constexpr uint32_t rgba() const noexcept;
	constexpr uint32_t bgra() const noexcept;

	constexpr uint8_t red() const noexcept;
	constexpr uint8_t green() const noexcept;
	constexpr uint8_t blue() const noexcept;
	constexpr uint8_t alpha() const noexcept;

	constexpr generic_color_rgb to_generic_rgb() const noexcept;
};


struct color_hsv_t
{
	int16_t m_hue; //0 - 359
	uint8_t m_saturation; //0 - 100
	uint8_t m_value; //0 - 100


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
	int16_t m_hue; //0 - 359
	uint8_t m_saturation; //0 - 100
	uint8_t m_value; //0 - 100
	uint8_t m_alpha; //0 - 255



	constexpr int16_t hue() const noexcept;
	constexpr uint8_t saturation() const noexcept;
	constexpr uint8_t value() const noexcept;
	constexpr uint8_t alpha() const noexcept;

	constexpr void hue(uint16_t hue) noexcept;
	constexpr void saturation(uint8_t saturation) noexcept;
	constexpr void value(uint8_t value) noexcept;
	constexpr void alpha(uint8_t alpha) const noexcept;



	constexpr color_hsva_t() noexcept;
	constexpr color_hsva_t(uint16_t hue, uint8_t saturation, uint8_t value) noexcept;

	template<color color_t>
	constexpr color_hsva_t(color_t other) noexcept;

	constexpr color_hsva_t(const color_hsva_t&) noexcept = default;
	constexpr color_hsva_t(color_hsva_t&&) noexcept = default;

};


struct color_hsv_packed_t
{
	//uint16_t hue : 10; //0 - 359
	//uint8_t saturation : 7; //0-100
	//uint8_t value : 7; //0-100
	//24 bits total

	//What was the point of introducing a bit fields in the language if they are not packed

	uint8_t m_data[3];



	constexpr int16_t hue() const noexcept;
	constexpr uint8_t saturation() const noexcept;
	constexpr uint8_t value() const noexcept;

	constexpr void hue(int16_t hue) noexcept;
	constexpr void saturation(uint8_t saturation) noexcept;
	constexpr void value(uint8_t value) noexcept;



	constexpr color_hsv_packed_t() noexcept;
	constexpr color_hsv_packed_t(int16_t hue, uint8_t saturation, uint8_t value) noexcept;

	template<color color_t>
	constexpr color_hsv_packed_t(color_t other) noexcept;

	constexpr color_hsv_packed_t(const color_hsv_packed_t&) noexcept = default;
	constexpr color_hsv_packed_t(color_hsv_packed_t&&) noexcept = default;

};


struct color_hsva_packed_t
{
	uint32_t m_data;



	constexpr int16_t hue() const noexcept;
	constexpr uint8_t saturation() const noexcept;
	constexpr uint8_t value() const noexcept;
	constexpr uint8_t alpha() const noexcept;

	constexpr void hue(uint16_t hue) noexcept;
	constexpr void saturation(uint8_t saturation) noexcept;
	constexpr void value(uint8_t value) noexcept;
	constexpr void alpha(uint8_t alpha) noexcept;



	constexpr color_hsva_packed_t() noexcept;
	constexpr color_hsva_packed_t(uint16_t hue, uint8_t saturation, uint8_t value) noexcept;
	constexpr color_hsva_packed_t(uint16_t hue, uint8_t saturation, uint8_t value, uint8_t alpha) noexcept;

	template<color color_t>
	constexpr color_hsva_packed_t(color_t other) noexcept;

	constexpr color_hsva_packed_t(const color_hsva_packed_t&) noexcept = default;
	constexpr color_hsva_packed_t(color_hsva_packed_t&&) noexcept = default;

};


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

_KSN_DETAIL_BEGIN

constexpr int _linear_360_oscilator_abs(int x)
{
	return x < 0 ? -x : x;
}

constexpr int linear_360_oscilator(int x)
{
	x %= 360;
	if (x < 0)
		x += 360;
	return _linear_360_oscilator_abs(180 - x);
}

constexpr int convert_hsv_rgb_oscilator(int x)
{
	return linear_360_oscilator(x) + linear_360_oscilator(x + 120) - 120;
}

constexpr int convert_hsv_rgb_helper(int x, int S, int V)
{
	x = V * (12000 - S * (120 - x));
	int q = x / 80000;
	int r = x % 80000;
	if (r >= 40000)
		q = q + 1;
	return q;
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
constexpr generic_color_rgb generic_color_convert_rgb(color_t other)
{
	return detail::generic_color_convert_rgb1(other);
}
template<color_rgb color_t>
constexpr generic_color_rgb generic_color_convert_rgb(color_t other)
{
	return other.to_generic_rgb();
}

_KSN_END





_KSN_BEGIN

constexpr color_bgr_t::color_bgr_t() noexcept
{
	this->r = this->b = this->g = 0;
}
constexpr color_bgr_t::color_bgr_t(uint8_t r, uint8_t g, uint8_t b) noexcept
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
constexpr color_bgra_t::color_bgra_t(uint8_t r, uint8_t g, uint8_t b) noexcept
{
	this->b = b;
	this->g = g;
	this->r = r;
	this->a = 255;
}
constexpr color_bgra_t::color_bgra_t(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept
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
	//TODO: implement generic_color_convert_hsv
	throw;
}

constexpr generic_color_hsv color_hsv_t::to_generic_hsv() const noexcept
{
	generic_color_hsv val;
	val.m_hue = this->m_value;
	val.m_saturation = this->m_saturation;
	val.m_value = this->m_value;
	val.alpha = 255;
	return val;
}

_KSN_END
