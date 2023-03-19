
export module libksn.index_pool;

import <ksn/ksn.hpp>;

import <numeric>;
import <algorithm>;
import <functional>;
import <vector>;
import <random>;



_KSN_EXPORT_BEGIN


thread_local std::mt19937_64 index_pool_rng(std::random_device{}());

class index_pool
{
	std::vector<size_t> idx;
public:
	template<class Rng>
	void reset(size_t size, size_t start, Rng&& rng);
	void reset(size_t size, size_t start = 0);

	size_t operator()() noexcept;
};


_KSN_EXPORT_END




_KSN_BEGIN

template<class Rng>
inline void index_pool::reset(size_t size, size_t start, Rng&& rng)
{
	idx.resize(size);
	std::ranges::iota(this->idx, start);
	std::ranges::shuffle(this->idx, rng);
}

inline void index_pool::reset(size_t size, size_t start)
{
	this->reset(size, start, index_pool_rng);
}

inline size_t index_pool::operator()() noexcept
{
	size_t result = this->idx.back();
	this->idx.pop_back();
	return result;
}

_KSN_END
