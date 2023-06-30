
export module libksn.math.long_integer;

import <ksn/ksn.hpp>;
import <intrin.h>;



_KSN_EXPORT_BEGIN

template<class S>
concept long_integer_storage =
	requires(S & s, const S & cs, size_t n)
{
	{ s[n] } -> std::same_as<uint64_t&>;
	{ cs[n] } -> std::same_as<const uint64_t&>;
	{ cs.size() } -> std::same_as<size_t>;
	{ cs.capacity() } -> std::same_as<size_t>;
};

template<class S>
concept long_integer_resizable_storage = long_integer_storage<S> && 
	requires(S & s, size_t n, uint64_t fill)
{
	{ s.resize(n) } -> std::same_as<void>;
	{ s.resize(n, fill) } -> std::same_as<void>;
	{ s.reserve(n) } -> std::same_as<void>;
};



template<bool p_is_signed, long_integer_storage storage_t>
class long_integer
{
	using my_t = long_integer<p_is_signed, storage_t>;

private:
	storage_t m_storage;

public:
	using storage_type = storage_t;

	constexpr long_integer() = default;
	constexpr long_integer(const my_t&) = default;
	constexpr long_integer(my_t&&) = default;

	template<bool signed2, class st2>
	constexpr long_integer(const long_integer<signed2, st2>&);

	constexpr my_t& operator=(const my_t&) = default;
	constexpr my_t& operator=(my_t&&) = default;

	consteval static bool is_signed() noexcept;
	consteval static bool is_resizable() noexcept;
	constexpr size_t actual_size() const noexcept;
	constexpr size_t size() const noexcept;
	constexpr size_t capacity() const noexcept;

	constexpr int64_t sign_i64() const noexcept;
	constexpr uint64_t sign_u64() const noexcept;

	constexpr auto& operator[](size_t n) noexcept;
	constexpr auto& operator[](size_t n) const noexcept;

	template<class = void> 
	void resize(size_t) requires long_integer_resizable_storage<storage_t>;
	template<class = void> 
	void reserve(size_t) requires long_integer_resizable_storage<storage_t>;
	template<class = void> 
	void shrink() requires long_integer_resizable_storage<storage_t>;

	void flip_bits();
	void negate();

	my_t& operator++();
	my_t& operator--();

	template<bool sign2, class st2>
	constexpr my_t& operator+=(const long_integer<sign2, st2>& x);
	template<bool sign2, class st2>
	constexpr my_t& operator-=(const long_integer<sign2, st2>& x);
	template<bool sign2, class st2>
	constexpr my_t& operator*=(const long_integer<sign2, st2>& x);
};



template<size_t N>
struct long_integer_static_fixed_storage
{
	uint64_t data[N]{};

	constexpr uint64_t& operator[](size_t n) noexcept;
	constexpr const uint64_t& operator[](size_t n) const noexcept;
	consteval static size_t size() noexcept;
	consteval static size_t capacity() noexcept;
};

template<class container_t = std::vector<uint64_t>>
struct long_integer_dynamic_resizable_storage
{
	container_t data;

	constexpr uint64_t& operator[](size_t n) noexcept;
	constexpr const uint64_t& operator[](size_t n) const noexcept;
	constexpr size_t size() const noexcept;
	constexpr size_t capacity() const noexcept;
	constexpr void resize(size_t n);
	constexpr void resize(size_t n, uint64_t fill);
	constexpr void reserve(size_t n);
};



using uint128 = long_integer<false, long_integer_static_fixed_storage<2>>;
using int128 = long_integer<true, long_integer_static_fixed_storage<2>>;
using uint256 = long_integer<false, long_integer_static_fixed_storage<4>>;
using int256 = long_integer<true, long_integer_static_fixed_storage<4>>;

using long_uint = long_integer<false, long_integer_dynamic_resizable_storage<>>;
using long_int = long_integer<true, long_integer_dynamic_resizable_storage<>>;

_KSN_EXPORT_END



_KSN_BEGIN

template<size_t N>
constexpr uint64_t& long_integer_static_fixed_storage<N>::operator[](size_t n) noexcept
{
	return this->data[n];
}
template<size_t N>
constexpr const uint64_t& long_integer_static_fixed_storage<N>::operator[](size_t n) const noexcept
{
	return this->data[n];
}
template<size_t N>
inline consteval size_t long_integer_static_fixed_storage<N>::size() noexcept
{
	return N;
}
template<size_t N>
inline consteval size_t long_integer_static_fixed_storage<N>::capacity() noexcept
{
	return N;
}

template<class container_t>
constexpr uint64_t& long_integer_dynamic_resizable_storage<container_t>::operator[](size_t n) noexcept
{
	return this->data[n];
}
template<class container_t>
constexpr const uint64_t& long_integer_dynamic_resizable_storage<container_t>::operator[](size_t n) const noexcept
{
	return this->data[n];
}
template<class container_t>
inline constexpr size_t long_integer_dynamic_resizable_storage<container_t>::size() const noexcept
{
	return this->data.size();
}

template<class container_t>
inline constexpr size_t long_integer_dynamic_resizable_storage<container_t>::capacity() const noexcept
{
	return this->data.capacity();
}
template<class container_t>
inline constexpr void long_integer_dynamic_resizable_storage<container_t>::resize(size_t n)
{
	this->data.resize(n);
}
template<class container_t>
inline constexpr void long_integer_dynamic_resizable_storage<container_t>::resize(size_t n, uint64_t fill)
{
	this->data.resize(n, fill);
}
template<class container_t>
inline constexpr void long_integer_dynamic_resizable_storage<container_t>::reserve(size_t n)
{
	this->data.reserve(n);
}



constexpr int64_t cvt_sign64(uint64_t x)
{
	return (int64_t)x >> 63;
}
constexpr uint64_t cvt_sign64u(uint64_t x)
{
	return (uint64_t)cvt_sign64(x);
}



template<bool p_is_signed, long_integer_storage storage_t>
inline consteval bool long_integer<p_is_signed, storage_t>::is_signed() noexcept
{
	return p_is_signed;
}

template<bool p_is_signed, long_integer_storage storage_t>
inline consteval bool long_integer<p_is_signed, storage_t>::is_resizable() noexcept
{
	return long_integer_resizable_storage<storage_t>;
}

template<bool p_is_signed, long_integer_storage storage_t>
constexpr size_t long_integer<p_is_signed, storage_t>::actual_size() const noexcept
{
	size_t nonsign_idx = this->size() - 1;
	uint64_t x;
	while (nonsign_idx != -1)
	{
		x = (*this)[nonsign_idx];
		if (x != this->sign_u64())
			break;
		--nonsign_idx;
	}
	if constexpr (!p_is_signed)
		return nonsign_idx + 1;

	x = cvt_sign64u(x);
	if (x == this->sign_u64())
		return nonsign_idx + 1;
	else
		return nonsign_idx + 2;
}

template<bool p_is_signed, long_integer_storage storage_t>
inline constexpr size_t long_integer<p_is_signed, storage_t>::size() const noexcept
{
	return this->m_storage.size();
}

template<bool p_is_signed, long_integer_storage storage_t>
inline constexpr size_t long_integer<p_is_signed, storage_t>::capacity() const noexcept
{
	return this->m_storage.capacity();
}

template<bool p_is_signed, long_integer_storage storage_t>
inline constexpr int64_t long_integer<p_is_signed, storage_t>::sign_i64() const noexcept
{
	if constexpr (!p_is_signed)
		return 0;
	if (this->size() == 0)
		return 0;
	return cvt_sign64((*this)[this->size() - 1]);
}
template<bool p_is_signed, long_integer_storage storage_t>
inline constexpr uint64_t long_integer<p_is_signed, storage_t>::sign_u64() const noexcept
{
	return (uint64_t)this->sign_i64();
}

template<bool p_is_signed, long_integer_storage storage_t>
constexpr auto& long_integer<p_is_signed, storage_t>::operator[](size_t n) noexcept
{
	return this->m_storage[n];
}
template<bool p_is_signed, long_integer_storage storage_t>
constexpr auto& long_integer<p_is_signed, storage_t>::operator[](size_t n) const noexcept
{
	return this->m_storage[n];
}

template<bool p_is_signed, long_integer_storage storage_t>
void long_integer<p_is_signed, storage_t>::flip_bits()
{
	for (size_t i = 0; i < this->size(); ++i)
		(*this)[i] ^= -1;
}

template<bool p_is_signed, long_integer_storage storage_t>
void long_integer<p_is_signed, storage_t>::negate()
{
	this->flip_bits();
	++*this;
}

template<bool p_is_signed, long_integer_storage storage_t>
template<bool signed2, class st2>
constexpr long_integer<p_is_signed, storage_t>::long_integer(const long_integer<signed2, st2>& x)
{
	if constexpr (this->is_resizable())
	{
		if (x.size() == 0)
			return;

		const uint64_t sign2 = x.sign_u64();
		uint64_t sign1;
		if constexpr (p_is_signed)
			sign1 = cvt_sign64u(x[x.size() - 1]);
		else
			sign1 = sign2;

		size_t n = x.size();
		if (sign1 != sign2)
			++n;
		this->resize(n);
		memcpy(&(*this)[0], &x[0], x.size() * sizeof(uint64_t));
		if (sign1 != sign2)
			(*this)[n - 1] = sign2;
	}
	else
	{
		size_t n = std::min(this->size(), x.size());
		memcpy(&(*this)[0], &x[0], n * sizeof(uint64_t));
		memset(&(*this)[n], (int)x.sign_u64(), (this->size() - n) * sizeof(uint64_t));
	}
}

template<bool p_is_signed, long_integer_storage storage_t>
template<class>
void long_integer<p_is_signed, storage_t>::resize(size_t n) requires long_integer_resizable_storage<storage_t>
{
	if constexpr (!p_is_signed)
	{
		this->m_storage.resize(n);
		return;
	}
	
	const size_t old_n = this->size();
	if (n <= old_n)
	{
		this->m_storage.resize(n);
		return;
	}

	if (this->sign_u64())
		this->m_storage.resize(n, (uint64_t)-1);
	else
		this->m_storage.resize(n);
}
template<bool p_is_signed, long_integer_storage storage_t>
template<class>
void long_integer<p_is_signed, storage_t>::reserve(size_t n) requires long_integer_resizable_storage<storage_t>
{
	this->m_storage.reserve(n);
}

template<bool p_is_signed, long_integer_storage storage_t>
template<class>
void long_integer<p_is_signed, storage_t>::shrink() requires long_integer_resizable_storage<storage_t>
{
	this->resize(this->actual_size());
}





constexpr inline uint8_t addcarry(uint8_t carry, uint64_t a, uint64_t b, uint64_t* result)
{
	using ull = unsigned long long;

	if (!std::is_constant_evaluated())
		return (uint8_t)_addcarry_u64((unsigned char)carry, (ull)a, (ull)b, (ull*)result);

	*result = a + b + carry;

	const uint64_t limit = UINT64_MAX - carry;
	if (a > b)
		std::swap(a, b);
	if (limit < a)
		return 1;
	if (b > limit - a)
		return 1;
	return 0;
}
constexpr inline uint8_t subborrow(uint8_t carry, uint64_t a, uint64_t b, uint64_t* result)
{
	using ull = unsigned long long;

	if (!std::is_constant_evaluated())
		return (uint8_t)_subborrow_u64((unsigned char)carry, (ull)a, (ull)b, (ull*)result);

	*result = a - b - carry;

	if (b == UINT64_MAX)
		return 1;
	b += carry;
	return b > a;
}

template<bool add>
constexpr inline uint8_t addsubcarry(uint8_t carry, uint64_t a, uint64_t b, uint64_t* result)
{
	if constexpr (add)
		return addcarry(carry, a, b, result);
	else
		return subborrow(carry, a, b, result);
}



template<bool add, bool p_is_signed, long_integer_storage storage_t>
void inc_dec(long_integer<p_is_signed, storage_t>& c)
{
	constexpr bool add = true;

	const uint64_t sign1 = c.sign_u64();
	const size_t n = c.size();

	unsigned char carry = 1;
	for (size_t i = 0; carry && i < n; ++i)
		carry = addsubcarry<add>(carry, 0, c[i], &c[i]);

	if constexpr (c.is_resizable() && c.is_signed())
	{
		bool condition;
		if constexpr (add)
			condition = sign1 == 0;
		else
			condition = sign1 != 0;

		if (condition && sign1 != c.sign_u64())
		{
			c.resize(n + 1);
			c[n] = sign1;
		}
	}
	else if constexpr (c.is_resizable() && !c.is_signed())
	{
		bool condition;
		if constexpr (add)
			condition = sign1 == 0 && carry;
		else
			condition = false;

		if (condition)
		{
			c.resize(n + 1);
			c[n] = 1;
		}
	}
}



template<
	bool add,
	bool signed1, class st1,
	bool signed2, class st2,
	bool signed3, class st3>
constexpr void addsub(
	const long_integer<signed1, st1>& a,
	const long_integer<signed2, st2>& b,
	long_integer<signed3, st3>& c)
{
	size_t n1 = a.size(), n2 = b.size();
	size_t n;
	
	if constexpr (add)
	{
		if (n1 < n2)
			return addsub<add>(b, a, c);
	}

	const uint64_t sign1 = a.sign_u64();
	const uint64_t sign2 = b.sign_u64();

	if constexpr (c.is_resizable())
	{
		n = std::max(n1, n2);
		c.reserve(n + 1);
		c.resize(n);
	}
	else
	{
		n = c.size();
		n1 = std::min(n1, n);
		n2 = std::min(n2, n);
	}

	unsigned char carry = 0;
	size_t i = 0;

	if (n1 < n2)
	{
		for (; i < n1; ++i)
			carry = addsubcarry<add>(carry, a[i], b[i], &c[i]);
		for (; i < n2; ++i)
			carry = addsubcarry<add>(carry, sign1, b[i], &c[i]);
	}
	else
	{
		for (; i < n2; ++i)
			carry = addsubcarry<add>(carry, a[i], b[i], &c[i]);
		for (; i < n1; ++i)
			carry = addsubcarry<add>(carry, a[i], sign2, &c[i]);
	}

	if constexpr (c.is_resizable() && c.is_signed())
	{
		bool condition;
		if constexpr (add)
			condition = sign1 == sign2;
		else
			condition = sign1 != sign2;

		if (condition && sign1 != c.sign_u64())
		{
			c.resize(n + 1);
			c[n] = sign1;
		}
	}
	else if constexpr (c.is_resizable() && !c.is_signed())
	{
		bool condition;
		if constexpr (add)
			condition = sign1 == sign2 && carry;
		else
			condition = !sign1 && sign2 && !carry;

		if (condition)
		{
			c.resize(n + 1);
			c[n] = 1;
		}
	}
	else
	{
		for (; i < n; ++i)
			carry = addsubcarry<add>(carry, sign1, sign2, &c[i]);
	}
}



#define lodword(x) uint32_t(x)
#define hidword(x) uint32_t(uint64_t(x) >> 32)
constexpr inline uint64_t umul128(uint64_t a, uint64_t b, uint64_t* h)
{
	using ull = unsigned long long;
	if (!std::is_constant_evaluated())
		return (uint64_t)_umul128((ull)a, (ull)b, (ull*)h);

	uint32_t out[4]{};
	uint32_t in1[2]{}, in2[2]{};
	in1[0] = lodword(a);
	in1[1] = hidword(a);
	in2[0] = lodword(b);
	in2[1] = hidword(b);

	uint64_t l1l2 = (uint64_t)in1[0] * in2[0];
	uint64_t l1h2 = (uint64_t)in1[0] * in2[1];
	uint64_t h1l2 = (uint64_t)in1[1] * in2[0];
	uint64_t h1h2 = (uint64_t)in1[1] * in2[1];

	auto addcarry32 = [] 
	(uint8_t c, uint32_t a, uint32_t b, uint32_t& s) 
	{
		uint64_t _s = uint64_t(a) + b + c;
		s = lodword(_s);
		c = (uint8_t)hidword(_s);
		return c;
	};
	auto add_at = [&](size_t n, uint64_t x)
	{
		uint8_t c = 0;
		c = addcarry32(c, lodword(x), out[n], out[n]);
		c = addcarry32(c, hidword(x), out[n + 1], out[n + 1]);
		for (size_t i = n + 2; i < 4; ++i)
			c = addcarry32(c, 0, out[i], out[i]);
	};
	add_at(0, l1l2);
	add_at(1, l1h2);
	add_at(1, h1l2);
	add_at(2, h1h2);
	*h = ((uint64_t)out[3] << 32) + out[2];
	return ((uint64_t)out[1] << 32) + out[0];
}

template<
	bool signed1, class st1,
	bool signed2, class st2,
	bool signed3, class st3>
constexpr void long_multiply(
	const long_integer<signed1, st1>& a,
	const long_integer<signed2, st2>& b,
	long_integer<signed3, st3>& _KSN_RESTRICT c)
{
	const size_t N = a.size();
	const size_t M = b.size();
	const size_t L = c.size();
	const uint64_t sign1 = a.sign_u64();
	const uint64_t sign2 = b.sign_u64();

	for (size_t i = 0; i < L; ++i)
	{
		uint64_t multiplication_carry = 0;
		uint8_t addition_carry = 0;
		for (size_t j = 0; j < L - i; ++j)
		{
			uint64_t l, h;
			l = umul128(
				i < N ? a[i] : sign1,
				j < M ? b[j] : sign2,
				&h
			);

			addition_carry = addcarry(addition_carry, l + multiplication_carry, c[j + i], &c[j + i]);
			multiplication_carry = h;
		}
	}
}

template<
	bool signed1, class st1,
	bool signed2, class st2,
	bool signed3, class st3>
void multiply(
	const long_integer<signed1, st1>& a,
	const long_integer<signed2, st2>& b,
	long_integer<signed3, st3>& _KSN_RESTRICT c)
{
	if constexpr (c.is_resizable())
		c.resize(a.size() + b.size());

	long_multiply(a, b, c);

	if constexpr (c.is_resizable())
		c.resize(c.actual_size());
}

_KSN_END



_KSN_BEGIN

template<long_integer_storage st1, long_integer_storage st2>
struct combined_storage
{
	using type = st1;
};

template<long_integer_storage st1, long_integer_resizable_storage st2> requires(!long_integer_resizable_storage<st1>)
struct combined_storage<st1, st2>
{
	using type = st2;
};

template<long_integer_storage st1, long_integer_storage st2>
using combined_storage_t = combined_storage<st1, st2>::type;

template<bool sign1, bool sign2, class st1, class st2>
using combined_integer_t = long_integer<sign1&& sign2, combined_storage_t<st1, st2>>;

_KSN_END



_KSN_EXPORT_BEGIN

template<bool p_is_signed, long_integer_storage storage_t>
template<bool sign2, class st2>
constexpr auto long_integer<p_is_signed, storage_t>::operator+=(const long_integer<sign2, st2>& x) -> my_t&
{
	addsub<true>(*this, x, *this);
	return *this;
}
template<bool p_is_signed, long_integer_storage storage_t>
template<bool sign2, class st2>
constexpr auto long_integer<p_is_signed, storage_t>::operator-=(const long_integer<sign2, st2>& x) -> my_t&
{
	addsub<false>(*this, x, *this);
	return *this;
}

template<bool p_is_signed, long_integer_storage storage_t>
template<bool sign2, class st2>
constexpr auto long_integer<p_is_signed, storage_t>::operator*=(const long_integer<sign2, st2>& x) -> my_t&
{
	my_t tmp;
	multiply(*this, x, tmp);
	*this = std::move(tmp);
	return *this;
}

template<bool sign1, class st1, bool sign2, class st2>
constexpr auto operator+(const long_integer<sign1, st1>& a, const long_integer<sign2, st2>& b)
{
	combined_integer_t<sign1, sign2, st1, st2> result;
	addsub<true>(a, b, result);
	return result;
}
template<bool sign1, class st1, bool sign2, class st2>
constexpr auto operator-(const long_integer<sign1, st1>& a, const long_integer<sign2, st2>& b)
{
	combined_integer_t<sign1, sign2, st1, st2> result;
	addsub<false>(a, b, result);
	return result;
}
template<bool sign1, class st1, bool sign2, class st2>
constexpr auto operator*(const long_integer<sign1, st1>& a, const long_integer<sign2, st2>& b)
{
	combined_integer_t<sign1, sign2, st1, st2> result;
	multiply(a, b, result);
	return result;
}

template<bool p_is_signed, long_integer_storage storage_t>
auto long_integer<p_is_signed, storage_t>::operator++() -> my_t&
{
	inc_dec<true>(*this);
	return *this;
}
template<bool p_is_signed, long_integer_storage storage_t>
auto long_integer<p_is_signed, storage_t>::operator--() -> my_t&
{
	inc_dec<false>(*this);
	return *this;
}

_KSN_EXPORT_END



_KSN_BEGIN

template<uint64_t divisor, bool is_signed, class storage_t>
uint64_t single_digit_udivide_and_get_remainder_ct(ksn::long_integer<is_signed, storage_t>& x)
{
	uint64_t carry = 0;
	for (size_t i = x.size() - 1; i != -1; --i)
		x[i] = _udiv128(carry, x[i], divisor, &carry);
	return carry;
}

export template<bool is_signed, class char_t, class traits_t>
auto& operator<<(std::basic_ostream<char_t, traits_t>& os, ksn::long_integer<is_signed, long_integer_dynamic_resizable_storage<>> x)
{
	x.shrink();

	bool negative = x.sign_u64();
	if (negative)
		x.negate();

	std::vector<char_t> v;
	v.reserve(x.size() * 20);

	static constexpr uint64_t M = 10000;
	static constexpr int k = 4;

	extern const char put10_4[];

	while (x.size())
	{
		if (x.size() == 1 && x[0] < 1000)
			break;
		uint64_t rem = single_digit_udivide_and_get_remainder_ct<M>(x);
		auto* p = &put10_4[(size_t)rem * k];
		v.append_range(std::span<const char, 4>(p, p + 4));
		x.shrink();
	}
	uint64_t xi = x.size() ? x[0] : 0;
	while (xi)
	{
		uint64_t rem = xi % 10;
		xi /= 10;
		v.push_back('0' + (char)rem);
	}

	if (negative)
		v.push_back('-');

	std::ranges::reverse(v);
	os << std::basic_string_view<char_t, traits_t>(v.data(), v.size());

	return os;
}

export template<bool is_signed, class storage_t, class char_t, class traits_t>
auto& operator<<(std::basic_ostream<char_t, traits_t>& os, const ksn::long_integer<is_signed, storage_t>& x)
{
	return (os << (ksn::long_integer<is_signed, long_integer_dynamic_resizable_storage<>>)x);
}

_KSN_END
