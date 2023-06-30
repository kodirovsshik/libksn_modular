



export module libksn.math.wilcoxon;

import <ksn/ksn.hpp>;
import libksn.mdvector;




_KSN_EXPORT_BEGIN

template<class fp_t = double>
class wilcoxon_Udistribution
{
	std::vector<fp_t> m_data;
	size_t reflection_point;

public:
	constexpr wilcoxon_Udistribution(size_t m, size_t n);

	constexpr fp_t pdf(size_t x) const noexcept;
	constexpr fp_t cdf(size_t x) const noexcept;
	constexpr size_t quantile(fp_t x) const;
	template<class Rng> requires(std::uniform_random_bit_generator<Rng>)
	constexpr fp_t random(Rng&&) const noexcept;
};

_KSN_EXPORT_END





_KSN_BEGIN

template<class fp_t = double>
auto get_wilcox_counts(size_t m, size_t n)
{
	const size_t S = m * n / 2;

	ksn::multivector<fp_t, 2> dp;
	dp.resize({ n + 1, S + 1 });

	for (size_t i = 0; i <= n; ++i)
		dp[i][0] = 1;

	for (size_t i = 1; i <= m; ++i)
	{
		for (size_t j = 1; j <= n; ++j)
		{
			for (size_t k = S; k != -1; --k)
			{
				dp[j][k] = dp[j - 1][k];
				if (k >= j)
					dp[j][k] += dp[j][k - j];
			}
		}
	}

	std::vector<fp_t> v;
	v.reserve(S + 2);
	v.resize(S + 1);
	for (size_t k = 0; k <= S; ++k)
		v[k] = dp[n][k];
	return v;
}

template<class fp_t = double>
auto get_wilcox_pdf(size_t m, size_t n)
{
	if (n > m)
		std::swap(n, m);

	auto v = get_wilcox_counts(m, n);

	fp_t total_outcomes = 1;
	for (size_t k = 1; k <= n; ++k)
		total_outcomes = total_outcomes * (m + k) / k;

	for (auto& x : v)
		x /= total_outcomes;
	return v;
}
template<class fp_t = double>
auto get_wilcox_cdf(size_t m, size_t n)
{
	auto v = get_wilcox_pdf(m, n);

	fp_t last = std::move(v.back());
	std::exclusive_scan(v.begin(), v.end(), v.begin(), 0.);
	last += v.back();
	v.emplace_back(std::move(last));
	return v;
}

_KSN_END





_KSN_EXPORT_BEGIN

template<class fp_t>
constexpr wilcoxon_Udistribution<fp_t>::wilcoxon_Udistribution(size_t m, size_t n)
	: reflection_point(m * n)
{
	this->m_data = get_wilcox_cdf(m, n);
}

template<class fp_t>
constexpr fp_t wilcoxon_Udistribution<fp_t>::pdf(size_t x) const noexcept
{
	if (x > this->reflection_point)
		return 0;
	x = std::min(x, this->reflection_point - x);
	return this->m_data[x + 1] - this->m_data[x];
}

template<class fp_t>
constexpr fp_t wilcoxon_Udistribution<fp_t>::cdf(size_t x) const noexcept
{
	if (x > this->reflection_point)
		return 1;
	const size_t x2 = this->reflection_point - x;
	return (x <= x2) ? this->m_data[x + 1] : (1 - this->m_data[x2]);
}

template<class fp_t>
constexpr size_t wilcoxon_Udistribution<fp_t>::quantile(fp_t p) const
{
	if (p > 0.5)
		return this->reflection_point - this->quantile(1 - p);
	
	auto pos = std::ranges::lower_bound(this->m_data.begin() + 1, this->m_data.end(), p);
	return pos - this->m_data.begin() - 1;
}

_KSN_EXPORT_END
