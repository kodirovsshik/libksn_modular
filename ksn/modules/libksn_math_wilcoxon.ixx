
export module libksn.math:wilcoxon;

import libksn.mdvector;
import <vector>;
import <memory>;
import <ranges>;

import <ksn/ksn.hpp>;



_KSN_BEGIN

_KSN_DETAIL_BEGIN

struct wilcoxon_row_storage
{
	std::unique_ptr<double[]> data;
	size_t size = 0;

	double& operator[](size_t n)
	{
		if (n >= this->size && _KSN_IS_DEBUG_BUILD)
			__debugbreak();
		return this->data[n];
	}

	void allocate(size_t n)
	{
		this->data = std::make_unique<double[]>(n);
		this->size = n;
	}
	void set(uint8_t byte)
	{
		memset(this->data.get(), byte, this->size * sizeof(double));
	}

	auto view()
	{
		return std::ranges::views::counted(this->data.get(), this->size);
	}
};

_KSN_DETAIL_END

_KSN_END



_KSN_EXPORT_BEGIN


class wilcoxon_Udistribution_data
{
	multivector<detail::wilcoxon_row_storage, 2> m_data;
	double cached_inverse_combinations_count;
	unsigned m_max_sum, m_n;

	double data(int S, int m, int n);
	double& data_ref(int S, int m, int n);

	static constexpr double calc_inverse_combinations_count(unsigned n, unsigned m);
	double inverse_combinations_count(unsigned n, unsigned m);

public:
	wilcoxon_Udistribution_data(unsigned max_n, unsigned max_m);

	double density(unsigned S);
	double density(unsigned S, unsigned n, unsigned m);

	unsigned n() const noexcept;
	unsigned m() const noexcept;

	std::vector<size_t> _storage_usage_statistics();
};


_KSN_EXPORT_END





_KSN_BEGIN

wilcoxon_Udistribution_data::wilcoxon_Udistribution_data(unsigned n, unsigned m)
{
	this->m_max_sum = m * n;
	this->m_n = n;
	this->m_data.resize({ this->m_max_sum / 2 + (size_t)1, (size_t)m + 1 });
	
	for (unsigned a = 0; a <= m; ++a)
	{
		auto& row = this->m_data.data()[a];
		row.allocate(n + 1);
		
		for (auto& x : row.view())
			x = 1;
	}

	this->cached_inverse_combinations_count = calc_inverse_combinations_count(n, m);
}

double wilcoxon_Udistribution_data::density(unsigned S)
{
	return this->density(S, this->n(), this->m());
}
double wilcoxon_Udistribution_data::density(unsigned S, unsigned n, unsigned m)
{
	return this->data(std::min(S, this->m_max_sum - S), n, m) * this->inverse_combinations_count(n, m);
}

unsigned wilcoxon_Udistribution_data::n() const noexcept
{
	return this->m_n;
}
unsigned wilcoxon_Udistribution_data::m() const noexcept
{
	return static_cast<unsigned>(this->m_data.size_dims()[1] - 1);
}

std::vector<size_t> wilcoxon_Udistribution_data::_storage_usage_statistics()
{
	size_t alloc = 0;
	size_t alloc_max = 0;
	size_t used = 0;
	size_t used_max = 0;
	for (auto& x : this->m_data)
	{
		alloc += x.size;
		alloc_max += this->m_n + 1;
		for (auto y : x.view())
			used += (y != y);
		used_max += x.size;
	}
	return { alloc, alloc_max, used, used_max };
}


double wilcoxon_Udistribution_data::data(int S, int m, int n)
{
	if (S < 0 || n < 0 || m < 0)
		return 0;

	auto& val = data_ref(S, m, n);

	if (val != val)
		val = this->data(S - n, m - 1, n) + this->data(S, m, n - 1);
		//val = data(S - n, m - 1, n - 1) + data(S, m, n - 1); //Why u wrong
	return val;
}

double& wilcoxon_Udistribution_data::data_ref(int S, int m, int n)
{
	auto& row = this->m_data[S][m];
	if (row.size == 0)
	{
		row.allocate(this->m_n + 1);
		row.set(0xFF);
	}
	return row[n];
}

constexpr double wilcoxon_Udistribution_data::calc_inverse_combinations_count(unsigned n, unsigned m)
{
	if (n < m)
		return calc_inverse_combinations_count(m, n);

	unsigned num = 1;
	unsigned denom = n + 1;

	double result = 1;
	while (num <= m)
	{
		result *= num++;
		result /= denom++;
	}

	return result;
}
double wilcoxon_Udistribution_data::inverse_combinations_count(unsigned n, unsigned m)
{
	if (n == this->n() && m == this->m())
		return this->cached_inverse_combinations_count;
	return calc_inverse_combinations_count(n, m);
}

_KSN_END
