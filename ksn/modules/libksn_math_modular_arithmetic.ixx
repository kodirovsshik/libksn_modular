
export module libksn.math.modular_arithmetic;

import <ksn/ksn.hpp>;

//TODO: provide custom totient function parameter?



_KSN_BEGIN

static constexpr bool modular_integer_check_divisibility =
#if defined KSN_MODULAR_INTEGER_CHECK_DIVISIBILITY
true
#elif defined KSN_MODULAR_INTEGER_DONT_CHECK_DIVISIBILITY
false
#else
(bool)_KSN_IS_DEBUG_BUILD
#endif
;

_KSN_END



_KSN_EXPORT_BEGIN

//Modulus must not exceed (max(T)+1)/2
template<class T, T mod>
class modular_integer
{
	using my_t = modular_integer<T, mod>;
	static_assert(mod > 0, "");

	static constexpr bool nothrow_copy_constructible = std::is_nothrow_copy_constructible_v<T>;

	T m_value;

public:
	constexpr modular_integer() noexcept = default;
	constexpr modular_integer(const T&) noexcept(nothrow_copy_constructible);

	constexpr my_t& operator+=(const my_t&) noexcept;
	constexpr my_t& operator-=(const my_t&) noexcept;
	constexpr my_t& operator*=(const my_t&) noexcept;
	constexpr my_t& operator/=(const my_t&) noexcept(!modular_integer_check_divisibility);
	constexpr my_t& operator%=(const my_t&) noexcept;

	constexpr my_t operator+(const my_t&) const noexcept;
	constexpr my_t operator-(const my_t&) const noexcept;
	constexpr my_t operator*(const my_t&) const noexcept;
	constexpr my_t operator/(const my_t&) const noexcept(!modular_integer_check_divisibility);
	constexpr my_t operator%(const my_t&) const noexcept;

	constexpr my_t operator+() const noexcept;
	constexpr my_t operator-() const noexcept;

	constexpr my_t& operator++() noexcept;
	constexpr my_t& operator--() noexcept;

	constexpr my_t operator++(int) noexcept;
	constexpr my_t operator--(int) noexcept;

	constexpr operator T() const noexcept;
};


template<class T, T mod>
modular_integer<T, mod> pow(const modular_integer<T, mod>& a, const modular_integer<T, mod>& b);

template<class T, T mod>
modular_integer<T, mod> pow(const modular_integer<T, mod>& a, const T& b);


_KSN_EXPORT_END





_KSN_BEGIN


_KSN_DETAIL_BEGIN

template<class T>
constexpr bool _is_prime(const T& x) noexcept
{
	for (T i = 2; i < x; ++i)
	{
		if ((x % i) == 0)
			return false;
	}
	return true;
}


template<class T, T mod>
constexpr  T _calc_phi() noexcept
{
	T phi = mod;
	for (T i = 2; i <= mod; ++i)
	{
		if (_is_prime(i) && (mod % i) == 0)
			phi -= phi / i;
	}
	return phi;
}

_KSN_DETAIL_END


export
template<class T, T mod>
struct phi
{
	static constexpr T value = detail::_calc_phi<T, mod>();
};


_KSN_DETAIL_BEGIN

template<class T, T mod>
constexpr  T _phi() noexcept
{
	return phi<T, mod>::value;
}

template<class T, T mod>
constexpr modular_integer<T, mod> _inverse(const modular_integer<T, mod>& x)
{
	return pow(x, _phi<T, mod>() - 1);
}
template<class T, T mod>
constexpr T _inverse(const T& x)
{
	if (modular_integer_check_divisibility && std::gcd<T, T>(x, mod) != 1)
		throw std::domain_error("modular_integer: invalid division");
	return pow(modular_integer<T, mod>(x), _phi<T, mod>() - 1);
}

template<class T, T mod>
constexpr T _normalize(const T& x) noexcept
{
	if (x >= 0)
		return x % mod;
	else
	{
		return x - (x / mod + 1) * mod;
	}
}

template<class T, T mod>
constexpr T _multiply(const T& a, T b) noexcept
{
	if (a < b)
		return _multiply<T, mod>(b, a);
	T result{};
	T current = a;
	while (b)
	{
		if (b & 1)
			result = (result + current) % mod;
		b >>= 1;
		current = (current << 1) % mod;

	}
	return result;
}

template<class T, T mod>
constexpr T _divide(const T& a, const T& b)
{
	return _multiply<T, mod>(a, _inverse<T, mod>(b));
}

_KSN_DETAIL_END



template<class T, T mod>
constexpr modular_integer<T, mod>::modular_integer(const T& x) noexcept(nothrow_copy_constructible)
	: m_value(detail::_normalize<T, mod>(x))
{
}

template<class T, T mod>
constexpr modular_integer<T, mod>& modular_integer<T, mod>::operator+=(const my_t& rhs) noexcept
{
	this->m_value = (this->m_value + rhs.m_value) % mod;
	return *this;
}
template<class T, T mod>
constexpr modular_integer<T, mod>& modular_integer<T, mod>::operator-=(const my_t& rhs) noexcept
{
	this->m_value = (this->m_value + mod - rhs.m_value) % mod;
	return *this;
}
template<class T, T mod>
constexpr modular_integer<T, mod>& modular_integer<T, mod>::operator*=(const my_t& rhs) noexcept
{
	this->m_value = detail::_multiply<T, mod>(this->m_value, rhs.m_value);
	return *this;
}
template<class T, T mod>
constexpr modular_integer<T, mod>& modular_integer<T, mod>::operator/=(const my_t& rhs) noexcept(!modular_integer_check_divisibility)
{
	this->m_value = detail::_divide<T, mod>(this->m_value, rhs.m_value);
	return *this;
}
template<class T, T mod>
constexpr modular_integer<T, mod>& modular_integer<T, mod>::operator%=(const my_t& rhs) noexcept
{
	this->m_value %= rhs.m_value;
	return *this;
}

template<class T, T mod>
constexpr modular_integer<T, mod> modular_integer<T, mod>::operator+(const my_t& other) const noexcept
{
	my_t result(*this);
	result += other;
	return result;
}
template<class T, T mod>
constexpr modular_integer<T, mod> modular_integer<T, mod>::operator-(const my_t& other) const noexcept
{
	my_t result(*this);
	result -= other;
	return result;
}
template<class T, T mod>
constexpr modular_integer<T, mod> modular_integer<T, mod>::operator*(const my_t& other) const noexcept
{
	my_t result(*this);
	result *= other;
	return result;
}
template<class T, T mod>
constexpr modular_integer<T, mod> modular_integer<T, mod>::operator/(const my_t& other) const noexcept(!modular_integer_check_divisibility)
{
	my_t result(*this);
	result /= other;
	return result;
}
template<class T, T mod>
constexpr modular_integer<T, mod> modular_integer<T, mod>::operator%(const my_t& other) const noexcept
{
	my_t result(*this);
	result %= other;
	return result;
}

template<class T, T mod>
constexpr modular_integer<T, mod> modular_integer<T, mod>::operator+() const noexcept
{
	//bruh
	return *this;
}
template<class T, T mod>
constexpr modular_integer<T, mod> modular_integer<T, mod>::operator-() const noexcept
{
	//conditional move pretty please?
	return this->m_value ? my_t(mod - this->m_value) : my_t(0);
}

template<class T, T mod>
constexpr modular_integer<T, mod>& modular_integer<T, mod>::operator++() noexcept
{
	++this->m_value;
	if (this->m_value == mod)
		this->m_value = 0;
	return *this;
}
template<class T, T mod>
constexpr modular_integer<T, mod>& modular_integer<T, mod>::operator--() noexcept
{
	if (this->m_value == 0)
		this->m_value = mod;
	--this->m_value;
	return *this;
}
template<class T, T mod>
constexpr modular_integer<T, mod> modular_integer<T, mod>::operator++(int) noexcept
{
	my_t copy(*this);
	++*this;
	return copy;
}
template<class T, T mod>
constexpr modular_integer<T, mod> modular_integer<T, mod>::operator--(int) noexcept
{
	my_t copy(*this);
	--*this;
	return copy;
}

template<class T, T mod>
constexpr modular_integer<T, mod>::operator T() const noexcept
{
	return this->m_value;
}



template<class T, T mod>
modular_integer<T, mod> _pow(modular_integer<T, mod> a, T b)
{
	modular_integer<T, mod> result(T(1));

	while (b > 0)
	{
		if (b & 1)
			result *= a;
		b >>= 1;
		a *= a;
	}
	return result;
}

template<class T, T mod>
modular_integer<T, mod> pow(const modular_integer<T, mod>& a, const modular_integer<T, mod>& b)
{
	return _pow(a, (T)b);
}

template<class T, T mod>
modular_integer<T, mod> pow(const modular_integer<T, mod>& a, const T& b)
{
	if (b < 0)
		return _pow(_inverse(a), -b);
	return _pow(a, b);
}

_KSN_END
