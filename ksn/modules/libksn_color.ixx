
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
	int16_t m_hue; //0 - 359
	uint8_t m_saturation; //0 - 100
	uint8_t m_value; //0 - 100
	uint8_t alpha; //0 - 255
};

struct generic_color_hsv_packed
{
public:
	uint32_t data;
};


_KSN_EXPORT_END



_KSN_EXPORT_BEGIN


template<class T>
concept color_bgr = requires(T color)
{
	{ color.to_generic_bgr() } -> std::same_as<generic_color_bgr>;
	{ color.red() } -> std::same_as<uint8_t>;
	{ color.green() } -> std::same_as<uint8_t>;
	{ color.blue() } -> std::same_as<uint8_t>;
	{ color.alpha() } -> std::same_as<uint8_t>;
};
template<class T>
concept color_rgb = requires(T color)
{
	{ color.to_generic_rgb() } -> std::same_as<generic_color_rgb>;
	{ color.red() } -> std::same_as<uint8_t>;
	{ color.green() } -> std::same_as<uint8_t>;
	{ color.blue() } -> std::same_as<uint8_t>;
	{ color.alpha() } -> std::same_as<uint8_t>;
};
template<class T>
concept color_hsv = requires(T color)
{
	{ color.to_generic_hsv() } -> std::convertible_to<generic_color_hsv>;

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
concept color_hsv_packed = requires(T color)
{
	{ color.to_generic_hsv_packed() } -> std::convertible_to<generic_color_hsv_packed>;

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



_KSN_DETAIL_BEGIN

template<color color_t>
constexpr ksn::generic_color_bgr generic_color_convert_bgr3(color_t) noexcept
{
	return {};
}
template<color_hsv_packed color_t>
constexpr ksn::generic_color_bgr generic_color_convert_bgr3(color_t) noexcept
{
	return {};
}
template<color color_t>
constexpr ksn::generic_color_bgr generic_color_convert_bgr2(color_t) noexcept
{
	return {};
}
template<color_hsv color_t>
constexpr ksn::generic_color_bgr generic_color_convert_bgr2(color_t) noexcept
{
	return {};
}
template<color color_t>
constexpr ksn::generic_color_bgr generic_color_convert_bgr1(color_t other) noexcept
{
	return generic_color_convert_bgr2(other);
}
template<color_rgb color_t>
constexpr ksn::generic_color_bgr generic_color_convert_bgr1(color_t other) noexcept
{
	auto rgb = other.to_generic_rgb();

	generic_color_bgr bgr;
	bgr.r = rgb.r;
	bgr.g = rgb.g;
	bgr.b = rgb.b;
	bgr.a = rgb.a;

	return bgr;
}

_KSN_DETAIL_END

template<ksn::color color_t>
constexpr ksn::generic_color_bgr generic_color_convert_bgr(color_t other) noexcept
{
	return detail::generic_color_convert_bgr1(other);
}
template<ksn::color_bgr color_t>
constexpr ksn::generic_color_bgr generic_color_convert_bgr(color_t other) noexcept
{
	return other.to_generic_bgr();
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
