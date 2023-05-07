
export module libksn.math:long_integer;

import <ksn/ksn.hpp>;
import <concepts>;
import <vector>;
import <type_traits>;
import <algorithm>;



_KSN_BEGIN

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
	requires(S & s, size_t n)
{
	{ s.resize(n) } -> std::same_as<void>;
};


template<size_t N>
struct long_integer_static_fixed_storage
{
	uint64_t data[N]{};

	//constexpr auto&& operator[](this auto&& self, size_t n) noexcept
	//{
	//	return self.data[n];
	//}
	constexpr auto&& operator[](size_t n) noexcept
	{
		return this->data[n];
	}
	constexpr auto&& operator[](size_t n) const noexcept
	{
		return this->data[n];
	}
	consteval static size_t size() noexcept
	{
		return N;
	}
	consteval static size_t capacity() noexcept
	{
		return N;
	}
};

template<class container_t = std::vector<uint64_t>>
struct long_integer_dynamic_resizable_storage
{
	container_t data;

	//constexpr auto&& operator[](this auto&& self, size_t n) noexcept
	//{
	//	return this->data[n];
	//}
	constexpr auto&& operator[](size_t n) noexcept
	{
		return this->data[n];
	}
	constexpr auto&& operator[](size_t n) const noexcept
	{
		return this->data[n];
	}
	constexpr size_t size() const noexcept
	{
		return this->data.size();
	}
	constexpr size_t capacity() const noexcept
	{
		return this->data.capacity();
	}
	constexpr void resize(size_t n)
	{
		this->data.resize(n);
	}
};

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


_KSN_END



_KSN_EXPORT_BEGIN

template<bool p_is_signed, long_integer_storage storage_t>
class long_integer
{
	using my_t = long_integer<p_is_signed, storage_t>;

private:
	storage_t m_storage;

public:
	constexpr long_integer() = default;
	constexpr long_integer(const my_t&) = default;

	consteval static bool is_signed() noexcept;
	consteval static bool is_resizable() noexcept;
	constexpr size_t size() const noexcept;
	constexpr size_t capacity() const noexcept;

	constexpr int64_t sign_i64() const noexcept;
	constexpr uint64_t sign_u64() const noexcept;

	constexpr auto& operator[](size_t n) noexcept;
	constexpr auto& operator[](size_t n) const noexcept;

	template<class T = void> 
	void resize(size_t) requires long_integer_resizable_storage<storage_t>;

	template<bool sign2, class st2>
	constexpr my_t& operator+=(const long_integer<sign2, st2>& x);
	template<bool sign2, class st2>
	constexpr my_t& operator-=(const long_integer<sign2, st2>& x);

	template<bool sign1, class st1, bool sign2, class st2>
	constexpr friend auto operator+(const long_integer<sign1, st1>& a, const long_integer<sign2, st2>& b);
	template<bool sign1, class st1, bool sign2, class st2>
	constexpr friend auto operator-(const long_integer<sign1, st1>& a, const long_integer<sign2, st2>& b);
};

using uint128 = long_integer<false, long_integer_static_fixed_storage<2>>;
using int128 = long_integer<true, long_integer_static_fixed_storage<2>>;

using long_uint = long_integer<false, long_integer_dynamic_resizable_storage<>>;
using long_int = long_integer<true, long_integer_dynamic_resizable_storage<>>;

_KSN_EXPORT_END



_KSN_BEGIN

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
	return (int64_t)this->m_storage[this->size() - 1] >> 63;
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
template<class>
void long_integer<p_is_signed, storage_t>::resize(size_t n) requires long_integer_resizable_storage<storage_t>
{
	this->m_storage.resize(n);
}

_KSN_END



_KSN_BEGIN

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



export template<
	bool add,
	bool signed1, class st1,
	bool signed2, class st2,
	bool signed3, class st3>
void addsub(
	const long_integer<signed1, st1>& a,
	const long_integer<signed2, st2>& b,
	long_integer<signed3, st3>& c)
{
	size_t n1 = a.size(), n2 = b.size();
	size_t n;
	
	if (n1 < n2)
		return addsub<add>(b, a, c);

	if constexpr (c.is_resizable())
	{
		//resize C to max(a.size(), b.size())
		//fill C with a+b
		//extend if sign store needed

		n = std::max(n1, n2);
		c.resize(n + 1);
		c.resize(n);
	}
	else
	{
		//bound n1, n2 above by n
		//fill C with a+b
		//fill rest
		n = c.size();
		n1 = std::min(n1, n);
		n2 = std::min(n2, n);
	}

	const uint64_t sign1 = a.sign_u64();
	const uint64_t sign2 = b.sign_u64();

	unsigned char carry = 0;
	size_t i = 0;
	for (; i < n2; ++i)
		carry = addsubcarry<add>(carry, a[i], b[i], &c[i]);

	for (; i < n1; ++i)
		carry = addsubcarry<add>(carry, a[i], sign2, &c[i]);

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



template<bool sign1, class st1, bool sign2, class st2>
constexpr auto operator+(const long_integer<sign1, st1>& a, const long_integer<sign2, st2>& b)
{
	long_integer<sign1 && sign2, combined_storage_t<st1, st2>> result;
	addsub<true>(a, b, result);
	return result;
}
template<bool sign1, class st1, bool sign2, class st2>
constexpr auto operator-(const long_integer<sign1, st1>& a, const long_integer<sign2, st2>& b)
{
	long_integer<sign1 && sign2, combined_storage_t<st1, st2>> result;
	addsub<false>(a, b, result);
	return result;
}

_KSN_END
