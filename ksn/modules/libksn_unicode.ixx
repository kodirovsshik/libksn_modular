
export module libksn.unicode;

import libksn.type_traits;
import <ksn/ksn.hpp>;

import <bit>;
import <string>;
import <type_traits>;



_KSN_EXPORT_BEGIN

enum class bom : uint8_t
{
	keep, mandate, forbid
};

struct convert_result_t
{
	size_t read, written;
};

_KSN_EXPORT_END



_KSN_BEGIN

template<class char_t>
struct compatible_char
{
};
template<any_of<char8_t, char16_t, char32_t> T>
struct compatible_char<T>
	: std::type_identity<T>
{
};
template<>
struct compatible_char<char>
{
	using type = char8_t;
};
template<>
struct compatible_char<wchar_t>
{
	using type = std::conditional_t<
		sizeof(wchar_t) == sizeof(char16_t), 
		char16_t, 
		std::conditional_t<
			sizeof(wchar_t) == sizeof(char32_t),
			char32_t,
			void
		>
	>;
};

template<class char_t>
using compatible_char_t = compatible_char<char_t>::type;

template<class T>
void bswap(T* x)
{
	uint8_t* p = (uint8_t*)x;

	static constexpr size_t n = sizeof(T);
	for (size_t i = 0; i < n / 2; ++i)
	{
		uint8_t a = p[i];
		uint8_t b = p[n - i - 1];
		p[n - i - 1] = a;
		p[i] = b;
	}
}

template<class char_t>
bool is_big_endian(const char_t*, size_t) noexcept;

template<>
bool is_big_endian<char8_t>(const char8_t*, size_t) noexcept
{
	return std::endian::native == std::endian::big;
}

template<>
bool is_big_endian<char16_t>(const char16_t* p, size_t n) noexcept
{
	if (n >= 1 && *p == 0xFFFE)
		return std::endian::native == std::endian::little;
	return std::endian::native == std::endian::big;
}

template<>
bool is_big_endian<char32_t>(const char32_t* p, size_t n) noexcept
{
	if (n >= 1 && (*p & 0xFF000000) != 0)
		return std::endian::native == std::endian::little;
	return std::endian::native == std::endian::big;
}

template<>
bool is_big_endian<char>(const char* p, size_t n) noexcept
{
	return is_big_endian<char8_t>((const char8_t*)p, n);
}

template<>
bool is_big_endian<wchar_t>(const wchar_t* p, size_t n) noexcept
{
	using compatible = compatible_char_t<wchar_t>;
	return is_big_endian<compatible>((const compatible*)p, n);
}



template<class char_t>
bool is_non_native_endianness(const char_t* p, size_t n) noexcept
{
	return is_big_endian(p, n) != (std::endian::big == std::endian::native);
}


#define assert_or_invalid(expr) if (!(expr)) return (size_t)-1;
#define is_low_utf8_byte(expr) (((expr) & 0b11000000) == 0b10000000)
#define low_bits(expr, n) ((expr) & ((1 << n) - 1))

template<bool different_endianness, character char_t>
class character_converter
{
public:
	static size_t get(const char_t* data_ptr, size_t data_size, char32_t& result);
	static size_t put(char_t* data_ptr, size_t data_size, char32_t value);
};

template<bool different_endianness>
class character_converter<different_endianness, char8_t>
{
public:
	static size_t get(const char8_t* data_ptr, size_t data_size, char32_t& result)
	{
		//assert_or_invalid(data_ptr != nullptr);
		assert_or_invalid(data_size >= 1);

		auto p = (const uint8_t*)data_ptr;

		uint8_t b0 = *p++;
		if ((int8_t)b0 >= 0)
		{
			result = b0;
			return 1;
		}

		if ((b0 & 0b11100000) == 0b11000000)
		{
			assert_or_invalid(data_size >= 2);
			uint8_t b1 = *p++;
			assert_or_invalid(is_low_utf8_byte(b1));
			result = (low_bits(b0, 5) << 6) | low_bits(b1, 6);
			return 2;
		}
		else if ((b0 & 0b11110000) == 0b11100000)
		{
			assert_or_invalid(data_size >= 3);
			uint8_t b1 = *p++;
			uint8_t b2 = *p++;
			assert_or_invalid(is_low_utf8_byte(b1));
			assert_or_invalid(is_low_utf8_byte(b2));
			result = (low_bits(b0, 4) << 12) | (low_bits(b1, 6) << 6) | low_bits(b2, 6);
			return 3;
		}
		else
		{
			assert_or_invalid((b0 & 0b11111000) == 0b11110000);
			assert_or_invalid(data_size >= 4);
			uint8_t b1 = *p++;
			uint8_t b2 = *p++;
			uint8_t b3 = *p++;
			assert_or_invalid(is_low_utf8_byte(b1));
			assert_or_invalid(is_low_utf8_byte(b2));
			assert_or_invalid(is_low_utf8_byte(b3));
			result = (low_bits(b0, 3) << 18) | (low_bits(b1, 6) << 12) | (low_bits(b2, 6) << 6) | low_bits(b3, 6);
			return 4;
		}
	}

	static size_t put(char8_t* data_ptr, size_t data_size, char32_t value)
	{
		uint32_t x = value;
		
		if (x <= 0x7F)
		{
			if (data_size >= 1)
				*data_ptr = (char8_t)x;
			return 1;
		}
		else if (x <= 0x7FF)
		{
			if (data_size >= 2)
			{
				*data_ptr++ = ((char8_t)(x >> 6) & 0b00011111) | 0b11000000;
				*data_ptr++ = ((char8_t)(x) & 0b00111111) | 0b10000000;
			}
			return 2;
		}
		else if (x <= 0xFFFF)
		{
			if (data_size >= 3)
			{
				*data_ptr++ = ((char8_t)(x >> 11) & 0b00001111) | 0b11100000;
				*data_ptr++ = ((char8_t)(x >> 6) & 0b00111111) | 0b10000000;
				*data_ptr++ = ((char8_t)(x) & 0b00111111) | 0b10000000;
			}
			return 3;
		}
		else if (x <= 0x1FFFFF)
		{
			if (data_size >= 4)
			{
				*data_ptr++ = ((char8_t)(x >> 11) & 0b00000111) | 0b11110000;
				*data_ptr++ = ((char8_t)(x >> 11) & 0b00111111) | 0b10000000;
				*data_ptr++ = ((char8_t)(x >> 6) & 0b00111111) | 0b10000000;
				*data_ptr++ = ((char8_t)(x) & 0b00111111) | 0b10000000;
			}
			return 4;
		}
		
		assert_or_invalid(false);
	}
};

template<bool different_endianness>
class character_converter<different_endianness, char>
{
public:
	static size_t get(const char* data_ptr, size_t data_size, char32_t& result)
	{
		return character_converter<different_endianness, char8_t>::get((const char8_t*)data_ptr, data_size, result);
	}

	static size_t put(char* data_ptr, size_t data_size, char32_t value)
	{
		return character_converter<different_endianness, char8_t>::put((char8_t*)data_ptr, data_size, value);
	}
};

template<bool different_endianness>
class character_converter<different_endianness, char16_t>
{
public:
	static size_t get(const char16_t* data_ptr, size_t data_size, char32_t& result)
	{
		assert_or_invalid(data_size >= 1);

		auto p = (const uint16_t*)data_ptr;

		uint16_t w1, w2;
		w1 = *p++;
		if constexpr (different_endianness)
			bswap(&w1);

		if (w1 == 0xFEFF || w1 == 0xFFFE)
			return 1 + get(data_ptr + 1, data_size - 1, result);

		auto single_code_unit = [&]
		(uint16_t x)
		{
			result = (char32_t)x;
			return 1;
		};


		if ((w1 & 0b1111110000000000) == 1101110000000000)
			return single_code_unit(w1 & 0b1111111111);

		else if ((w1 & 0b1111110000000000) == 1101100000000000)
		{
			w2 = *p++;
			if constexpr (different_endianness)
				bswap(&w2);

			if ((w2 & 0b1111110000000000) == 1101110000000000)
			{
				result = (low_bits(w1, 10) << 10) | (low_bits(w2, 10));
				return 2;
			}
			else
				return single_code_unit(w1 & 0b1111111111);
		}
		else
			return single_code_unit(w1);
	}

	static size_t put(char16_t* data_ptr, size_t data_size, char32_t value)
	{
		uint32_t x = value;
		if (x <= 0xFFFF)
		{
			if (data_size >= 1)
			{
				*data_ptr = (char16_t)x;
				if constexpr (different_endianness)
					bswap(data_ptr);
			}
			return 1;
		}
		else if (x <= 0x10FFFF)
		{
			x -= 0x10000;
			if (data_size >= 2)
			{
				*data_ptr = 0xD800 + (x >> 10);
				if constexpr (different_endianness)
					bswap(data_ptr);

				++data_ptr;
				*data_ptr = 0xDC00 + (x & 1023);
				if constexpr (different_endianness)
					bswap(data_ptr);
			}
			return 2;
		}

		assert_or_invalid(false);
	}
};

template<bool different_endianness>
class character_converter<different_endianness, char32_t>
{
public:
	static size_t get(const char32_t* data_ptr, size_t data_size, char32_t& result)
	{
		assert_or_invalid(data_ptr != nullptr);
		assert_or_invalid(data_size >= 1);

		result = *data_ptr;
		if constexpr (different_endianness)
			bswap(&result);
		assert_or_invalid((result & 0xFFF0'0000) == 0);
		return 1;
	}

	static size_t put(char32_t* data_ptr, size_t data_size, char32_t value)
	{
		if (data_size >= 1)
		{
			*data_ptr = value;
			if constexpr (different_endianness)
				bswap(data_ptr);
		}
		return 1;
	}
};

template<bool different_endianness>
class character_converter<different_endianness, wchar_t>
{
public:
	static size_t get(const wchar_t* data_ptr, size_t data_size, char32_t& result)
	{
		using compatible = compatible_char_t<wchar_t>;
		using const_compatible = std::add_const_t<compatible>;
		using compatible_converter = character_converter<different_endianness, compatible>;
		return compatible_converter::get((const_compatible*)data_ptr, data_size, result);
	}

	static size_t put(wchar_t* data_ptr, size_t data_size, char32_t result)
	{
		using compatible = compatible_char_t<wchar_t>;
		using compatible_converter = character_converter<different_endianness, compatible>;
		return compatible_converter::put((compatible*)data_ptr, data_size, result);
	}
};



static constexpr size_t MAX_BOM = 3;

template<class char_t, bool swap>
class bom_provider
{
public:
	static size_t provide(char_t(&)[MAX_BOM]) noexcept;
};

template<bool swap>
class bom_provider<char8_t, swap>
{
public:
	static size_t provide(char8_t(&arr)[MAX_BOM]) noexcept
	{
		arr[0] = 0xEF;
		arr[1] = 0xBB;
		arr[2] = 0xBF;
		return 3;
	}
};

template<>
class bom_provider<char16_t, false>
{
public:
	static size_t provide(char16_t(&arr)[MAX_BOM]) noexcept
	{
		arr[0] = 0xFEFF;
		return 1;
	}
};

template<>
class bom_provider<char16_t, true>
{
public:
	static size_t provide(char16_t(&arr)[MAX_BOM]) noexcept
	{
		arr[0] = 0xFFFE;
		return 1;
	}
};

template<>
class bom_provider<char32_t, false>
{
public:
	static size_t provide(char32_t(&arr)[MAX_BOM]) noexcept
	{
		arr[0] = 0x0000FEFF;
		return 1;
	}
};

template<>
class bom_provider<char32_t, true>
{
public:
	static size_t provide(char32_t(&arr)[MAX_BOM]) noexcept
	{
		arr[0] = 0xFFFE0000;
		return 1;
	}
};



template<
	character output_t,
	bool swap
>
size_t get_bom(output_t(&arr)[MAX_BOM])
{
	using compatible = compatible_char_t<output_t>;
	return bom_provider<compatible, swap>::provide((compatible(&)[MAX_BOM])arr);
}

template<
	bool swap,
	character output_t,
	class output_traits_t,
	class output_allocator_t
>
void put_bom(std::basic_string<output_t, output_traits_t, output_allocator_t>& out)
{
	output_t bom[MAX_BOM];
	size_t bom_len = get_bom<output_t, swap>(bom);
	out.append(bom + 0, bom + bom_len);
}

template<
	bool swap,
	character output_t
>
void put_bom(output_t* &p, size_t& sz)
{
	output_t bom[MAX_BOM];
	size_t bom_len = get_bom<output_t, swap>(bom);

	bom_len = std::min(bom_len, sz);
	for (size_t i = 0; i < bom_len; ++i)
		*p++ = bom[i];

	sz -= bom_len;
}

template<class char_t>
bool has_bom(const char_t* p, size_t sz)
{
	char_t bom[MAX_BOM];
	size_t bom_len = get_bom<char_t, false>(bom);

	if (bom_len > sz)
		return false;

	if (std::equal(bom + 0, bom + bom_len, p))
		return true;

	get_bom<char_t, true>(bom);

	if (std::equal(bom + 0, bom + bom_len, p))
		return true;

	return false;
}



template<
	bool swap_in,
	bool swap_out,
	bom use_bom,
	character output_t,
	class output_traits_t,
	class output_allocator_t,
	character input_t,
	class input_traits_t,
	class input_allocator_t
>
convert_result_t
unicode_string_convert_loop(
	std::basic_string<output_t, output_traits_t, output_allocator_t>& out,
	const std::basic_string<input_t, input_traits_t, input_allocator_t>& in)
{
	out.clear();
	out.reserve(in.size() * sizeof(input_t) / sizeof(output_t)); //it's not a mistake

	const auto* data = in.data();
	size_t size = in.length();


	if (use_bom == bom::mandate || use_bom == bom::keep && has_bom(in.data(), in.length()))
		put_bom<swap_out>(out);

	while (true)
	{
		char32_t next;
		const size_t read_chars = character_converter<swap_in, input_t>::get(data, size, next);
		if (read_chars == -1)
		{
			out.clear();
			break;
		}

		if (next == 0)
			break;

		data += read_chars;

		output_t arr[4];
		const size_t put_chars = character_converter<swap_out, output_t>::put(arr, std::size(arr), next);
		if (put_chars == -1)
		{
			out.clear();
			break;
		}

		for (size_t i = 0; i < put_chars; ++i)
			out += arr[i];
	}


	convert_result_t result{};
	result.read = data - in.data();
	result.written = out.length();
	return result;
}


template<
	bool swap_in,
	bool swap_out,
	bom use_bom,
	character output_t,
	character input_t
>
convert_result_t unicode_string_convert_loop(output_t* _KSN_RESTRICT out, size_t outsz, const input_t* _KSN_RESTRICT in, size_t insz)
{
	if (outsz-- == 0)
		return { 0, 0 };

	auto* const out0 = out;
	auto* const in0 = in;

	if (use_bom == bom::mandate || use_bom == bom::keep && has_bom(in, insz))
		put_bom<swap_out>(out, outsz);

	while (true)
	{
		char32_t next;
		const size_t read_chars = character_converter<swap_in, input_t>::get(in, insz, next);
		if (read_chars == -1)
			break;

		if (next == 0)
			break;

		in += read_chars;
		insz -= read_chars;

		output_t arr[4];
		size_t put_chars = character_converter<swap_out, output_t>::put(arr, std::size(arr), next);
		if (put_chars == -1)
			break;

		put_chars = std::min(put_chars, outsz);
		for (size_t i = 0; i < put_chars; ++i)
			*out++ = arr[i];

		if ((outsz -= put_chars) == 0)
			break;
	}

	convert_result_t result{};
	result.read = in - in0;
	result.written = out - out0;
	*out++ = 0;
	return result;
}

_KSN_END





_KSN_EXPORT_BEGIN

template<
	bom use_bom = bom::keep,
	bool to_little_endian = std::endian::native == std::endian::little,
	character output_t,
	class output_traits_t,
	class output_allocator_t,
	character input_t,
	class input_traits_t,
	class input_allocator_t
>
convert_result_t
unicode_string_convert(
	std::basic_string<output_t, output_traits_t, output_allocator_t>& out,
	const std::basic_string<input_t, input_traits_t, input_allocator_t>& in)
{
	const bool swap_in = is_non_native_endianness(in.data(), in.length());
	constexpr bool swap_out = (std::endian::native == std::endian::big) == to_little_endian;
	if (swap_in)
		return unicode_string_convert_loop<true, swap_out, use_bom>(out, in);
	else
		return unicode_string_convert_loop<false, swap_out, use_bom>(out, in);
}

template<
	bom use_bom = bom::keep,
	character output_t,
	class output_traits_t,
	class output_allocator_t,
	character input_t,
	class input_traits_t,
	class input_allocator_t
>
convert_result_t
unicode_string_convert_le(
	std::basic_string<output_t, output_traits_t, output_allocator_t>& out,
	const std::basic_string<input_t, input_traits_t, input_allocator_t>& in)
{
	return unicode_string_convert<use_bom, true>(out, in);
}

template<
	bom use_bom = bom::keep,
	character output_t,
	class output_traits_t,
	class output_allocator_t,
	character input_t,
	class input_traits_t,
	class input_allocator_t
>
convert_result_t
unicode_string_convert_be(
	std::basic_string<output_t, output_traits_t, output_allocator_t>& out,
	const std::basic_string<input_t, input_traits_t, input_allocator_t>& in)
{
	return unicode_string_convert<use_bom, false>(out, in);
}


template<
	bom use_bom = bom::keep,
	bool to_little_endian = std::endian::native == std::endian::little,
	character output_t,
	character input_t
>
convert_result_t unicode_string_convert(output_t* _KSN_RESTRICT out, size_t outsz, const input_t* _KSN_RESTRICT in, size_t insz) noexcept
{
	const bool swap_in = is_non_native_endianness(in, insz);
	constexpr bool swap_out = (std::endian::native == std::endian::big) == to_little_endian;
	if (swap_in)
		return unicode_string_convert_loop<true, swap_out, use_bom>(out, outsz, in, insz);
	else
		return unicode_string_convert_loop<false, swap_out, use_bom>(out, outsz, in, insz);
}

template<
	bom use_bom = bom::keep,
	character output_t,
	character input_t
>
convert_result_t unicode_string_convert_le(output_t* _KSN_RESTRICT out, size_t outsz, const input_t* _KSN_RESTRICT in, size_t insz) noexcept
{
	return unicode_string_convert<use_bom, true>(out, outsz, in, insz);
}

template<
	bom use_bom = bom::keep,
	character output_t,
	character input_t
>
convert_result_t unicode_string_convert_be(output_t* _KSN_RESTRICT out, size_t outsz, const input_t* _KSN_RESTRICT in, size_t insz) noexcept
{
	return unicode_string_convert<use_bom, false>(out, outsz, in, insz);
}

_KSN_EXPORT_END
