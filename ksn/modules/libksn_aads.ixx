
export module libksn.aads;

import <ksn/ksn.hpp>;
import <atomic>;
import <stdlib.h>;
import <stdexcept>;



_KSN_EXPORT_BEGIN

template<class T, size_t N>
class lock_free_pod_ring
{
	struct alignas(8) index_pair
	{
		uint32_t begin;
		uint32_t size;
	};

	std::atomic<index_pair> indices{};
	std::atomic<T> ring[N]{};


public:
	size_t size() const
	{
		return indices.load().size;
	}
	void push(T x)
	{
		uint32_t new_idx;
		auto old_indices = indices.load();
		while (true)
		{
			auto local_indices = old_indices;
			if (local_indices.size < N)
			{
				new_idx = local_indices.begin + local_indices.size++;
			} 
			else
			{
				new_idx = local_indices.begin++;
				if (local_indices.begin == UINT32_MAX) local_indices.begin %= N;
			}
			if (this->indices.compare_exchange_weak(old_indices, local_indices))
				break;
		}
		ring[new_idx % N] = x;
	}
	T pop()
	{
		uint32_t target_idx;
		auto old_indices = indices.load();

		while (true)
		{
			auto local_indices = old_indices;
			if (local_indices.size == 0)
				throw std::out_of_range("lock_free_pod_ring::pop(): no elements available");

			local_indices.size--;
			target_idx = local_indices.begin++;
			if (local_indices.begin == UINT32_MAX) local_indices.begin %= N;

			if (this->indices.compare_exchange_weak(old_indices, local_indices))
				break;
		}

		return ring[target_idx % N];
	}
};

_KSN_EXPORT_END
